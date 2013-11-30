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
#include <butils/fn.h>

static const char *key_file_name, *in_file_name, *out_file_name;
static int verbose = 1;

#ifdef _TRACE
#define TRACE(...) do { fprintf (stderr, __VA_ARGS__); } while(0);
#else
#define TRACE(...) 
#endif

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

uint32_t get_file_size( FILE *fp )
{
   uint32_t length;

   fseek( fp, 0L, SEEK_END);
   //fgetpos( fp, &flen );
   length = ftell( fp );
   fseek( fp, 0L, SEEK_SET);

   return length;
}   

uint32_t fill_header( FILE *fp, ENC_HEADER *hdr )
{
   //fpons_t  flen;
   uint32_t length = get_file_size( fp );

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

#ifndef MULTI


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

   if (file_pos != file_size) {
      fprintf( stderr, "Error: did not read the whole file, File length %d, read %d\n", file_size - sizeof(ENC_HEADER), file_pos - sizeof(ENC_HEADER) );
      rt = -1;
      goto err;
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
#else 
//
// multi threaded case.
//

#include <pthread.h>
#include <tutils/tpool.h>
#include <tutils/tstart.h>
#include <cutils/heap.h>
#include <semaphore.h>
#include <errno.h>

#define MAX_PENDING_REQUEST 100

typedef enum {
  Encrypt_act,
  Decrypt_act,
} EAction;

typedef struct tagCRYPT_REQUEST {
  RUNNABLE base;
 
  EAction  action;
  int sequence_no;
  TQUEUE *response_queue;
 
  uint8_t *in_buf;
  uint32_t in_buf_len;
  uint32_t in_len;

  uint8_t *out_buf;
  uint32_t out_buf_len; // size of buffer
  uint32_t out_len; // size of data
  
  int action_status;
 
} CRYPT_REQUEST;

typedef struct tagRESULT_HANDLER_DATA {
  THREADPOOL *pool; 
  sem_t finish_notify;
  FILE *fpout;
  size_t file_size;
  EAction role;
  RSA *key;
  uint32_t key_size;
  TQUEUE response_queue;
  int  result_status;

} RESULT_HANDLER_DATA;


typedef struct tagHEAP_ENTRY {
  int sequence_no;
  CRYPT_REQUEST *r;

} HEAP_ENTRY;

int heap_compare( void *a, void *b, size_t entry )
{
  (void ) entry;

  HEAP_ENTRY *pa = (HEAP_ENTRY *) a;
  HEAP_ENTRY *pb = (HEAP_ENTRY *) b;

  if (pa->sequence_no < pb->sequence_no) {
    return -1;
  }
  if (pa->sequence_no > pb->sequence_no) {
    return 1;
  }
  return 0;
}

CRYPT_REQUEST *request_new( EAction action, int seq_no, TQUEUE *queue, uint32_t in_buf_size, uint32_t out_buf_size )
{
   CRYPT_REQUEST *req;

   req = (CRYPT_REQUEST *) malloc(sizeof( CRYPT_REQUEST ));

   if (action == Encrypt_act  || action == Decrypt_act)
   {
     req->in_buf = malloc( in_buf_size );
     req->in_buf_len = in_buf_size;

     req->out_buf = malloc( out_buf_size );
     req->out_buf_len = out_buf_size;

     req->out_len = req->in_len = 0;
   }     
   req->action_status = 0;

   req->action = action; //Encrypt_act;
   req->response_queue = queue; //&result_data.response_queue;
   req->sequence_no = seq_no;

   RUNNABLE_init( &req->base, NULL, NULL );

   return req;
}



void request_free(CRYPT_REQUEST *req)
{
    free( req->in_buf );
    free( req->out_buf );
    free( req );
}


void *tpool_init( void *tpool_ctx )
{
  RESULT_HANDLER_DATA *data = (RESULT_HANDLER_DATA *) tpool_ctx;
  RSA *res;

  if (data->role == Encrypt_act)
    res = RSAPublicKey_dup( data->key );
  else
    res = RSAPrivateKey_dup( data->key ); 
  RSA_blinding_off( res );
  return res;
}

void tpool_cleanup( void *thread_ctx, void *tpool_ctx )
{
  RSA *thread_key = (RSA *) thread_ctx;
  (void) tpool_ctx;
  RSA_free( thread_key );
}

// worker thread - encrypt or decrypt stuff.
void tpool_handle_request( struct tagRUNNABLE *request, void *thread_ctx, void *pool_ctx )
{
  CRYPT_REQUEST *r = (CRYPT_REQUEST *) request;
  RESULT_HANDLER_DATA *data = (RESULT_HANDLER_DATA *) pool_ctx;
  RSA *key = (RSA *) thread_ctx;

  TRACE("Worker start req %p-%d\n", r, r->sequence_no );
  
  if (data->result_status == 0) {
   switch( r->action ) {
    case Encrypt_act:
      r->out_len =  RSA_public_encrypt( (int) r->in_len, r->in_buf, r->out_buf, key, RSA_PKCS1_OAEP_PADDING);
      if (r->out_len != r->out_buf_len) {
	r->action_status = -1;
        fprintf( stderr, "Error: failed to encrypt seq=%d in-len=%d\n", r->sequence_no, r->in_len);   
      }	
      break;
    case Decrypt_act:
      r->out_len = RSA_private_decrypt( (int) r->in_len, r->in_buf, r->out_buf, key, RSA_PKCS1_OAEP_PADDING);
      if (r->out_len == (uint32_t) -1) {
	r->action_status = -1;
        fprintf( stderr, "Error: failed to decrypt seq=%d in-len=%d\n", r->sequence_no, r->in_len);   
      }
      break;
    default:  
      break;
   }
  } else {
    r->action_status = -1;
    request_free( r );
    TRACE("request cancelled req %p-%d status %d\n", r, r->sequence_no, r->action_status );
    return;
  }
  
  TRACE("Worker finish req %p-%d status %d\n", r, r->sequence_no, r->action_status );

  TQUEUE_push_block_on_queue_full( r->response_queue, &r->base );
}

int do_handle_result( RESULT_HANDLER_DATA *data, int is_decrypt, uint8_t *hash, size_t hash_len )

{
  HEAP heap;
  int prev_percent = 0, new_percent;
  size_t file_size, file_pos;
  CRYPT_REQUEST *r;
  SHA256_CTX sha;
  size_t nwritten;
  int sequence_no = 1;
  int rt = 0;
  int is_first = 1;

  SHA256_Init(&sha);
 
  file_size = data->file_size;
  file_pos = 0;
  HEAP_init( &heap, MAX_PENDING_REQUEST, sizeof(HEAP_ENTRY), heap_compare );
  
  do {
  
     r = (CRYPT_REQUEST *) TQUEUE_pop( &data->response_queue );
     
     if ( r == 0 ) {
        fprintf( stderr, "Error: Null message\n" );
	rt = -1;
	break;
     }

     TRACE("result handler: req %p-%d status %d cur-seq %d\n", r, r->sequence_no, r->action_status, sequence_no );

     if (r->action_status == -1) {
	fprintf(stderr,"Error: Failed to %s\n", is_decrypt ? "decrypt" : "encrypt" );
	rt = -1;
	break;
     }

     if (r->sequence_no != sequence_no) {
	HEAP_ENTRY entry;

	entry.sequence_no = r->sequence_no;
	entry.r = r;

        TRACE("result handler: PUSH req %p-%d\n", r, r->sequence_no );
	HEAP_push( &heap, &entry, sizeof( entry ) );
	r = NULL;
     } else {
	sequence_no ++;
     }	

     while(file_pos != data->file_size && rt == 0) {
       ENC_HEADER hdr;

       if (r == NULL) {
         HEAP_ENTRY *entry;

         if (!HEAP_size( & heap )) 
           break;

         entry = (HEAP_ENTRY *) HEAP_top( &heap );
         r = entry->r;
 
         TRACE("result handler: TOP req %p-%d cur-seq %d\n", r, r->sequence_no, sequence_no );
  
         if (entry->sequence_no !=  sequence_no) 
           break;

         HEAP_pop( &heap );
    
	 TRACE("result handler: POP req %p-%d\n", r, r->sequence_no );
   
         sequence_no ++;
       }	

 
       TRACE("result handler: HANDLE req %p-%d seq-no %d \n", r, r->sequence_no, sequence_no );

       // got encrypted data block.
       if (is_decrypt) {

	 SHA256_Update(&sha, r->in_buf, r->in_len);
       
         if (is_first) {
	   // first block includes the header.
	   is_first = 0;
	 
	   memcpy( &hdr, r->out_buf, sizeof(ENC_HEADER) );
	   header_to_host( &hdr ); 	

	   if (hdr.file_magic != FILE_MAGIC) {
	     fprintf( stderr, "Error: decryption key does not match the encryption key; after decryption the file header is invalid\n" );
	      rt = -1;
	      break;
	   }
	   file_size = hdr.data_length;

	   nwritten  = sizeof( ENC_HEADER );
           nwritten += fwrite( r->out_buf + sizeof(ENC_HEADER), 1, r->out_len - sizeof(ENC_HEADER), data->fpout );
	 } 
	 else
	 {
           nwritten = fwrite( r->out_buf, 1, r->out_len, data->fpout );
	 }

       } else {
         SHA256_Update(&sha, r->out_buf, r->out_len);
         
	 nwritten = fwrite( r->out_buf, 1, r->out_len, data->fpout );
       }
       
 
       if (feof( data->fpout )) {
         fprintf( stderr, "Error: Failed to write output file\n" );
	 rt = -1;
         break;
       }
  
       if (verbose) {
 	 new_percent = (int) ( ((double) file_pos / file_size) * 100 );
	 if (new_percent != prev_percent) {
	    fprintf( stderr, "%d%c\r", new_percent, '%' );
	    prev_percent = new_percent;
	 }
       }	

       if (is_decrypt) {
	 file_pos += nwritten;
       } else {
	 file_pos += r->in_len;
       }
       request_free( r );
       	
       TRACE( "result handler: FILEPOS %u FROM %u\n", file_pos, data->file_size );
       if (file_pos == file_size) {
         break;
       }
       r = NULL;
    }        
  } while( file_pos != file_size );  


  // clean pending entries on error
  if (rt == -1) {
    void *ret;

    while( TQUEUE_pop_non_blocking( &data->response_queue, &ret ) == 0) {
      if (ret) {
        request_free( (CRYPT_REQUEST *) ret );
      }
    }
  }

  SHA256_Final(hash, &sha);
  
  if (is_decrypt) {
      CRYPT_REQUEST *r;
      
      if (HEAP_size( &heap )) {
        HEAP_ENTRY *entry = (HEAP_ENTRY *) HEAP_top( &heap );
        r = entry->r;
      } else {
	r = (CRYPT_REQUEST *) TQUEUE_pop( &data->response_queue );
      }	

      if (!r) {
	 fprintf(stderr, "Error: missing message authentication\n" ) ;
	 rt = -1;
      } else if (r->out_len != hash_len || memcmp( hash, r->out_buf, hash_len ) != 0) {
	 fprintf(stderr, "Error: message authentication failed; hash over encrypted message does not match the stored hash %d %d.\n", r->out_len, hash_len  );
	 rt = -1;
         request_free( r );
       }
  }

  HEAP_free( &heap );

  return rt;
}


void *handle_decrypt_output( void *arg )
{
  RESULT_HANDLER_DATA *data = (RESULT_HANDLER_DATA *) arg;
  uint8_t hash[SHA256_DIGEST_LENGTH];
  int rt = 0;
 
  rt = do_handle_result( data, 1, hash, sizeof( hash ) );

  TRACE("result handler: EOF status=%d\n", rt );
 
  data->result_status = rt;
  sem_post( &data->finish_notify );

  return 0;
}

// handle output of worker thread.
void *handle_encrypt_output( void *arg )
{
  RESULT_HANDLER_DATA *data = (RESULT_HANDLER_DATA *) arg;
  uint8_t hash[SHA256_DIGEST_LENGTH];
  FILE *fpout;
  size_t nwritten;
  int rt = 0;

  fpout = data->fpout;

      rt = do_handle_result( data, 0, hash, sizeof( hash ) );

      if (rt == 0) {
	  void *out_buf = malloc(  data->key_size );
	  int out_len;

	  out_len = RSA_public_encrypt((int) sizeof(hash), hash, out_buf, data->key, RSA_PKCS1_OAEP_PADDING);
	  if ( out_len != (int) data->key_size ) {
	      fprintf( stderr, "Error: failed to encrypt;\n ");   
	      rt = -1;
	      free( out_buf );
	      goto err;
	   }
	   nwritten = fwrite( out_buf, 1, out_len, fpout );
	   if (nwritten != (size_t) out_len ||ferror(fpout)) {
	      fprintf( stderr, "Error: failed to write the encrypted file.\n" );
	      rt = -1;
	      free( out_buf );
	      goto err;
	   }	
	   free( out_buf );
       }

err:
  

  TRACE("result handler: EOF status=%d\n", rt );
 
  data->result_status = rt;
  sem_post( &data->finish_notify );

  return 0;
}


int  cleanup( RESULT_HANDLER_DATA *res_data, THREADPOOL *pool, int rt )
{
  // wait for response handler thread to finish its part.
  while( sem_wait( &res_data->finish_notify ) == -1 && errno == EINTR);

  // close worker threads.
  THREADPOOL_close( pool );
 
  if (res_data->result_status != 0) {
    rt = res_data->result_status;
  }    
  
  TRACE("Main thread: EOF status %d\n", rt );
  
  return rt;
}


int encrypt_file_multi( RSA *key, FILE *fpin, FILE *fpout )
{
  THREADPOOL *pool;
  pthread_t result_thread;
  RESULT_HANDLER_DATA res_data;
  ENC_HEADER enc_header;
  uint32_t key_size, in_buf_size;
  int seq_no = 1;
  int first = 1, rt = 0;
  size_t file_size, file_pos, in_read = 0;

  key_size = RSA_size(key);
  in_buf_size = key_size - PKCS_PADDING_SIZE; 
  
 
  file_size = fill_header( fpin, &enc_header );
  file_pos = 0;

  // create result handling thread
  res_data.result_status = 0;
  res_data.fpout = fpout;
  res_data.role = Encrypt_act;
  res_data.key = key;
  res_data.key_size = key_size;
  res_data.file_size = file_size;
  TQUEUE_init( &res_data.response_queue, MAX_PENDING_REQUEST );
  sem_init( &res_data.finish_notify, 0, 0 );
  
  pool = THREADPOOL_init_ext( tpool_handle_request, tpool_init, tpool_cleanup, 100, -2, -1, &res_data );
  if (!pool) {
    fprintf(stderr, "Error: Can't initialize thread pool\n" );
    return -1;
  }
  TRACE("worker thread pool started\n" );
  
  pthread_create_detached( &result_thread, 0, handle_encrypt_output, &res_data );
  TRACE("result thread started\n" );


  // read input data and post work requests.
  while( ! feof( fpin ) && res_data.result_status  == 0 ) {
    CRYPT_REQUEST *req;

    req = request_new( Encrypt_act, seq_no++, &res_data.response_queue, in_buf_size, key_size );
    if  (!req) {
      fprintf( stderr, "Error: failed to allocate request sequence_no=%d\n", seq_no);
      rt = -1;
      break;
    }

    if (first) {
	memcpy( req->in_buf, &enc_header, sizeof(ENC_HEADER) ); 

	in_read = fread( req->in_buf + sizeof(ENC_HEADER), 1 , req->in_buf_len - sizeof(ENC_HEADER), fpin );
	in_read += sizeof(ENC_HEADER);
	first = 0;
    } else {
	in_read = fread( req->in_buf, 1 , req->in_buf_len, fpin ); 
    }

    if (ferror(fpin)) {
      fprintf( stderr, "Error: failed to read inut file\n" );
      rt = -1;
      break;
    }

    req->in_len = in_read;

    TRACE("Send req %p-%d\n", req, req->sequence_no );
    THREADPOOL_send_block_on_queue_full( pool, &req->base ); 
    
    file_pos += in_read;
    if (file_pos == file_size) {
      break;
    }
  };

  if (rt) {
    TQUEUE_push_exit_message( &res_data.response_queue );
  }    
 
  return cleanup( &res_data, pool, rt );
}

int decrypt_file_multi( RSA *key, FILE *fpin, FILE *fpout )
{
  uint32_t key_size = RSA_size(key);
  THREADPOOL *pool;
  pthread_t result_thread;
  RESULT_HANDLER_DATA res_data;
  size_t file_size, file_pos;
  size_t in_read;
  int rt = 0;
  int seq_no = 1;

 
  file_size = get_file_size( fpin );
  file_pos = 0;

  // create result handling thread
  res_data.result_status = 0;
  res_data.fpout = fpout;
  res_data.role = Decrypt_act;
  res_data.key = key;
  res_data.key_size = key_size;
  res_data.file_size = file_size;
  TQUEUE_init( &res_data.response_queue, MAX_PENDING_REQUEST );
  sem_init( &res_data.finish_notify, 0, 0 );
  
  pthread_create_detached( &result_thread, 0, handle_decrypt_output, &res_data );
  TRACE("result thread started\n" );

  pool = THREADPOOL_init_ext( tpool_handle_request, tpool_init, tpool_cleanup, 100, -2, -1, &res_data );
  if (!pool) {
    fprintf(stderr, "Error: Can't initialize thread pool\n" );
    return -1;
  }
  TRACE("worker thread pool started\n" );
 
  while(  ! feof( fpin ) && res_data.result_status  == 0 && rt == 0 ) {
     CRYPT_REQUEST *req;
  
     req = request_new( Decrypt_act, seq_no++, &res_data.response_queue, key_size, key_size );
     if  (!req) {
       fprintf( stderr, "Error: failed to allocate request sequence_no=%d\n", seq_no);
       rt = -1;
       break;
     }

     in_read = fread( req->in_buf, 1 , key_size, fpin ); 

     if (in_read == 0 && feof( fpin )) {
       break;
     }

     if ( ferror(fpin) ) {
	fprintf( stderr, "Error: failed to read input file\n" );
	rt = -1;
	break;
     }
     req->in_len = in_read;
     file_pos += in_read;
     
     TRACE("Send req %p-%d size %d\n", req, req->sequence_no, in_read );
     THREADPOOL_send_block_on_queue_full( pool, &req->base ); 
  }

  if (file_pos != file_size) {
    fprintf(stderr,"Error: failed to read the whole file\n");
    rt = -1;
  } 

  if (rt) {
    TQUEUE_push_exit_message( &res_data.response_queue );
  }    
 
  return cleanup( &res_data, pool, rt );
}

//----------------------------------------------------------------------------------------

static pthread_mutex_t *lock_cs;
static long *lock_count;

unsigned long pthreads_thread_id(void)
	{
	unsigned long ret;

	ret=(unsigned long)pthread_self();
	return(ret);
	}

void pthreads_locking_callback(int mode, int type, char *file,
	     int line)
      {
      (void) file;
      (void) line;
#if 0
	fprintf(stderr,"thread=%4d mode=%s lock=%s %s:%d\n",
		CRYPTO_thread_id(),
		(mode&CRYPTO_LOCK)?"l":"u",
		(type&CRYPTO_READ)?"r":"w",file,line);
#endif
/*
	if (CRYPTO_LOCK_SSL_CERT == type)
		fprintf(stderr,"(t,m,f,l) %ld %d %s %d\n",
		CRYPTO_thread_id(),
		mode,file,line);
*/
	if (mode & CRYPTO_LOCK)
		{
		pthread_mutex_lock(&(lock_cs[type]));
		lock_count[type]++;
		}
	else
		{
		pthread_mutex_unlock(&(lock_cs[type]));
		}
	}



void thread_setup(void)
	{
	int i;

	lock_cs=OPENSSL_malloc(CRYPTO_num_locks() * sizeof(pthread_mutex_t));
	lock_count=OPENSSL_malloc(CRYPTO_num_locks() * sizeof(long));
	for (i=0; i<CRYPTO_num_locks(); i++)
		{
		    lock_count[i]=0;
		pthread_mutex_init(&(lock_cs[i]),NULL);
		}

	CRYPTO_set_id_callback(pthreads_thread_id);
	CRYPTO_set_locking_callback((void (*)())pthreads_locking_callback);
	}

void thread_cleanup(void)
	{
	int i;

	CRYPTO_set_locking_callback(NULL);
#if 0	
	fprintf(stderr,"cleanup\n");
#endif	
	for (i=0; i<CRYPTO_num_locks(); i++)
		{
		pthread_mutex_destroy(&(lock_cs[i]));
#if 0		
		fprintf(stderr,"%8ld:%s\n",lock_count[i],
			CRYPTO_get_lock_name(i));
#endif		
		}
	OPENSSL_free(lock_cs);
	OPENSSL_free(lock_count);
	}


#endif
//------------------------------------------------------------


void print_help(const char *argv0)
{
    char *fname = FN_file_name(  argv0 );

#ifdef ENCRYPT
    fprintf( stderr, "%s -k <public key file> -i <input file> [-o <output file>]\n"
		     "--------------------------------------------------------------\n"
		     "RSA encrypts and signs the input file, writes the output file\n" 
		     " -k		public key file in PEM format\n"
			 " -i <input file>	ciphertext, the output of rsaenc\n"
		     " -o <output file>	decrypted output\n"
		     " -v		verbose output (progress indicator)\n",

		     fname
		     
		     );
    exit(1);			

}
#else
    fprintf( stderr, "%s -k <private key file> -i <input file> [-o <output file>]\n"
		     "---------------------------------------------------------------\n"
		     "RSA decrypts and input file, verifies signature.\n"
		     "writes the output file\n" 
		     " -k		public key file in PEM format\n"
		     " -i <input file>	file to be encrypted\n"
		     " -o <output file> encrypted results\n"
		     " -s		silent mode - no progress indicator.\n",
 
		     fname
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
	  print_help( argv[0] );
	  break;
      }
    }

    if (!in_file_name || !key_file_name) {
	print_help( argv[0] );
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

#ifndef MULTI
    #ifdef ENCRYPT
	rt = encrypt_file( pkey, fpin, fpout );
    #else    
	rt = decrypt_file( pkey, fpin, fpout );
    #endif  
#else
    // don't want locks, but adds timing attach possibility
    //RSA_blinding_off( pkey );

    #ifdef ENCRYPT
        thread_setup();
	rt = encrypt_file_multi( pkey, fpin, fpout );
	thread_cleanup();
    #else    
        thread_setup();
	rt = decrypt_file_multi( pkey, fpin, fpout );
	thread_cleanup();
    #endif  
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
    if (rt) {
      exit( 1 );
    }
    return 0;
}
