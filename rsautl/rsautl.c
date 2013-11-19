#define _BSD_SOURCE 
#include <endian.h>

#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <malloc.h>
#include <openssl/bio.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/ssl.h>
#include <openssl/sha.h>

static const char *key_file_name, *in_file_name, *out_file_name;
static int verbose = 1;

char * read_file( const char *fname )
{
    FILE *fp;
    long fsize;
    char *ret;
    size_t ns;

    fp = fopen( fname, "r" );
    if (!fp)
	return 0;

    fseek( fp, 0L, SEEK_END);
    fsize = ftell( fp );
    fseek( fp, 0L, SEEK_SET);

    ret = (char *) malloc( fsize + 1 );
    if (!ret)
	return 0;

    ns = fread( ret, 1, fsize, fp );
    ret[ ns ] = '\0';

    fclose( fp );

    return ret;
}

RSA *read_PEM_pub_key(const char *fname)
{   
  char *skey;
  BIO *bio;   
  RSA *key = 0;
  int rt = 0;

  skey = read_file( fname );
  if (!skey) {
    fprintf( stderr, "Error: Failed to read file %s\n" , fname );
    return 0;
  }    

  bio = BIO_new(BIO_f_buffer());
  if (!bio) {
    return 0;
  }      

  if (BIO_set_buffer_read_data(bio, (void *) skey, strlen( skey )) == 1) {
    key = (void *) PEM_read_bio_RSA_PUBKEY( bio, NULL, NULL, NULL);
    if (key == 0) {
	rt = -1;
    }
  } else {
    rt = -1;
  }	
            
  BIO_free_all(bio);
  free( skey );

  return key;
}

RSA *read_PEM_priv_key(const char *fname)
{   
  char *skey;
  BIO *bio;   
  RSA *key = 0;
  int rt = 0;

  skey = read_file( fname );
  if (!skey) {
    return 0;
  }    

  bio = BIO_new(BIO_f_buffer());
  if (!bio) {
    return 0;
  }      

  if (BIO_set_buffer_read_data(bio, (void *) skey, strlen( skey )) == 1) {
    key = (void *) PEM_read_bio_RSAPrivateKey( bio, NULL, NULL, NULL);
    if (key == 0) {
	rt = -1;
    }
  } else {
    rt = -1;
  }	
            
  BIO_free_all(bio);
  free( skey );

  return key;
}

#define PKCS_PADDING_SIZE 42

#pragma pack(1)
typedef struct {
  uint32_t file_magic;
  uint32_t file_version;
  uint64_t data_length;

} ENC_HEADER;
#pragma pack()

#define FILE_MAGIC	0xABCDEF
#define FILE_VERSION_1  0x1

uint32_t fill_header( FILE *fp, ENC_HEADER *hdr )
{
   //fpos_t  flen;
   uint32_t length;

   fseek( fp, 0L, SEEK_END);
   //fgetpos( fp, &flen );
   length = ftell( fp );
   fseek( fp, 0L, SEEK_SET);

   hdr->file_magic =   htobe32( FILE_MAGIC );
   hdr->file_version = htobe32( FILE_VERSION_1 );
   hdr->data_length  = htobe64( length + sizeof(ENC_HEADER) );

   return length + sizeof(ENC_HEADER); 
}

void header_to_host( ENC_HEADER *hdr )
{
   hdr->file_magic =   be32toh( hdr->file_magic );
   hdr->file_version = be32toh( hdr->file_version );
   hdr->data_length  = be64toh( hdr->data_length );
}

int encrypt_file( RSA *key, FILE *fpin, FILE *fpout )
{
   int rt = 0;
   uint32_t key_size = RSA_size(key);
   uint8_t *in_buf, *out_buf;
   uint32_t in_buf_size, in_read, out_len, file_size, file_pos = 0;
   SHA256_CTX sha;
   uint8_t hash[SHA256_DIGEST_LENGTH];
   size_t nwritten;
   int first = 1;
   int prev_percent = 0, new_percent;

   SHA256_Init(&sha);

   in_buf_size = key_size - PKCS_PADDING_SIZE; 
   in_buf = (uint8_t *) malloc( in_buf_size ); 
   if (!in_buf) {
     return -1;
   }

   out_buf = (uint8_t *) malloc( key_size );
   if (!out_buf) {
     return -1;
   } 

   file_size = fill_header( fpin, (ENC_HEADER *) in_buf );
   file_pos = 0;

   while( 1 ) {
 
      if (first) {
	in_read = fread( in_buf + sizeof(ENC_HEADER), 1 , in_buf_size - sizeof(ENC_HEADER), fpin );
	in_read += sizeof(ENC_HEADER);
	first = 0;
      } else {
	in_read = fread( in_buf, 1 , in_buf_size, fpin ); 
      }

      file_pos += in_read;
 
      if ( ferror(fpin) ) {
       	fprintf( stderr, "Error: failed to read the input file.\n" );
	rt = -1;
	goto err;
      }

      out_len = RSA_public_encrypt((int) in_read, in_buf, out_buf, key, RSA_PKCS1_OAEP_PADDING);
      if ( out_len != key_size ) {
	fprintf( stderr, "Error: failed to encrypt\n" );
	rt = -1;
	goto err;
      }	
 
      SHA256_Update(&sha, out_buf, out_len);
     
      nwritten = fwrite( out_buf, 1, out_len, fpout );
      if (nwritten != (size_t) out_len || ferror(fpout)) {
	fprintf( stderr, "Error: failed to write the encrypted file.\n" );
	rt = -1;
	goto err;
      }	

      if (verbose) {
	new_percent = (int) ( ((double) file_pos / file_size) * 100 );
	if (new_percent != prev_percent) {
	    fprintf( stderr, "%d%c\r", new_percent, '%' );
	    prev_percent = new_percent;
	}
      }	

      if ( feof( fpin ) ) {
    	break;
      }
   }

   SHA256_Final(hash, &sha);
   out_len = RSA_public_encrypt((int) sizeof(hash), hash, out_buf, key, RSA_PKCS1_OAEP_PADDING);
   if ( out_len != key_size ) {
      fprintf( stderr, "Error: failed to encrypt;\n ");   
      rt = -1;
      goto err;
   }
   nwritten = fwrite( out_buf, 1, out_len, fpout );
   if (nwritten != (size_t) out_len ||ferror(fpout)) {
      fprintf( stderr, "Error: failed to write the encrypted file.\n" );
      rt = -1;
      goto err;
   }	

err:
   free(in_buf);
   free(out_buf);

   return rt;
}

int decrypt_file( RSA *key, FILE *fpin, FILE *fpout )
{
   int rt = 0;
   uint32_t key_size = RSA_size(key);
   uint8_t *in_buf, *out_buf;
   uint32_t in_buf_size, in_read, out_len;
   uint32_t file_pos = 0 , file_size = 0;
   ENC_HEADER hdr;
   SHA256_CTX sha;
   uint8_t hash[SHA256_DIGEST_LENGTH];
   uint64_t data_len = 0;
   int first = 1;
   int prev_percent = 0, new_percent;

   SHA256_Init(&sha);

   in_buf_size = key_size; // - PKCS_PADDING_SIZE; 
   in_buf = (uint8_t *) malloc( in_buf_size ); 
   if (!in_buf) {
     return -1;
   }     

   out_buf = (uint8_t *) malloc( key_size );
   if (!out_buf) {
     return -1;
   }     
   
   while( 1 ) {
      in_read = fread( in_buf, 1 , in_buf_size, fpin ); 

      if ( ferror(fpin) ) {
        fprintf( stderr, "Error: failed to read input file\n" );
        rt = -1;
	goto err;
      }
    
      SHA256_Update(&sha, in_buf, in_read);
  
      out_len = RSA_private_decrypt((int) in_read, in_buf, out_buf, key, RSA_PKCS1_OAEP_PADDING);
   
      if (out_len == (uint32_t) -1) {
	fprintf( stderr, "Error: can't decrypt\n" );
	rt = -1;
	goto err;
      }

      if (first) {
	memcpy( &hdr, out_buf, sizeof(ENC_HEADER) );
	header_to_host( &hdr ); 	

	if (hdr.file_magic != FILE_MAGIC) {
	    fprintf( stderr, "Error: decryption key does not match the encryption key; after decryption the file header is invalid\n" );
	    rt = -1;
	    goto err;
	}
	
	first = 0;
	data_len = hdr.data_length;
	file_size = hdr.data_length;

	fwrite( out_buf + sizeof(ENC_HEADER), 1, out_len - sizeof(ENC_HEADER), fpout );
      } else {
        fwrite( out_buf, 1, out_len, fpout );
      }	

      if (ferror(fpout)) {
        fprintf( stderr, "Error: Failed to write output file.\n" );
	rt = -1;
	goto err;
      }

      if (data_len < out_len) {
        data_len = out_len;
	rt = -1;
	goto err;
      }
      data_len -= out_len;
      if (data_len == 0) {
	break;
      }	


      if (verbose) {
	new_percent = (int) ( ((double) file_pos / file_size) * 100 );
	if (new_percent != prev_percent) {
	    fprintf( stderr, "%d%c\r", new_percent, '%' );
	    prev_percent = new_percent;
	}
      }	

      file_pos += out_len;

      if ( feof(fpin) ) {
	break;
      }	
   }

   if (data_len != 0) {
     fprintf( stderr, "Error: Encrypted file is too short\n" );
     rt = -1;
     goto err;
   }

   // read the checksum
   in_read = fread( in_buf, 1 , in_buf_size, fpin ); 

   if ( ferror(fpin) ) {
       fprintf(stderr, "Error: file without hash over encrypted message - missing message authentication\n" );
       rt = -1;
       goto err;
   }
   
   out_len = RSA_private_decrypt((int) in_read, in_buf, out_buf, key, RSA_PKCS1_OAEP_PADDING);
   if (out_len == (uint32_t) -1) {
	fprintf( stderr, "Error: can't decrypt %d\n", in_read );
	rt = -1;
	goto err;
   }

   SHA256_Final(hash, &sha);

   if (out_len != sizeof(hash) || memcmp( &hash, out_buf, sizeof(hash) ) != 0) {
        fprintf(stderr, "Error: message authentication failed; hash over encrypted message does not match the stored hash %d %d.\n", out_len, sizeof(hash)  );
	rt = -1;
	goto err;
    }	
 
err:
   free(in_buf);
   free(out_buf);

   return rt;
}

//------------------------------------------------------------

#ifdef ENCRYPT
void print_help()
{
    fprintf( stderr, "rsaenc -k <public key file> -i <input file> [-o <output file>]\n"
		     "--------------------------------------------------------------\n"
		     "RSA encrypts and signs the input file, writes the output file\n" 
		     " -k		public key file in PEM format\n"
		     " -i <input file>	ciphertext, the output of rsaenc\n"
		     " -o <output file>	decrypted output\n"
		     " -v		verbose output (progress indicator)\n"
		     
		     );
    exit(1);			

}
#else
void print_help()
{
    fprintf( stderr, "rsaenc -k <private key file> -i <input file> [-o <output file>]\n"
		     "---------------------------------------------------------------\n"
		     "RSA decrypts and input file, verifies signature.\n"
		     "writes the output file\n" 
		     " -k		public key file in PEM format\n"
		     " -i <input file>	file to be encrypted\n"
		     " -o <output file> encrypted results\n"
		     " -s		silent mode - no progress indicator.\n"
 
		     );
    exit(1);			

}
#endif

int parse_cmd_line(int argc, char *argv[])
{
    int c;

    while ( (c = getopt(argc, argv, "k:i:o:hs")) != -1) {
      switch(c) {
	case 'k':
	  key_file_name = strdup( optarg );
	  break;

	case 'i':
          in_file_name = strdup( optarg ); 
	  break;

	case 'o':
	  out_file_name = strdup( optarg );
	  break;

	case 's':
	  verbose = 0;
	  break;

	case 'h':
	  print_help();
	  break;
      }
    }

    if (!in_file_name || !key_file_name) {
	print_help();
        exit(1);
    }
    return 0;
} 

int main(int argc, char *argv[])
{
    RSA *pkey;
    FILE *fpin, *fpout;
    int rt = -1, open_out = 0;

    parse_cmd_line( argc, argv );

    fpin = fopen( in_file_name,"r" );
    if (!fpin) {
	fprintf( stderr, "Error: can't open input file %s\n", in_file_name );
	exit(1);
    }	

    if (!out_file_name) {
      fpout = stdout;
    } else {
      if ((fpout = fopen( out_file_name, "w" )) == 0) {
	fprintf( stderr, "Error: can't open file %s\n", out_file_name );
	exit(1);
      }
      open_out = 1;
    }      

    SSL_library_init();

#ifdef ENCRYPT
    pkey = read_PEM_pub_key( key_file_name );
#else    
    pkey = read_PEM_priv_key( key_file_name );
#endif  

    if (pkey) {
#ifdef ENCRYPT
	rt = encrypt_file( pkey, fpin, fpout );
#else    
	rt = decrypt_file( pkey, fpin, fpout );
#endif  
    } else {
	fprintf( stderr, "Error: Failed to read key file\n" );
    }

    RSA_free( pkey );


    fclose(fpin);
    if (open_out) {
      fclose(fpout);
      if (rt) {
	unlink( out_file_name );
      }
    }      

    return 0;
    }
