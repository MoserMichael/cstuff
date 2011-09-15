#!/usr/bin/perl


sub make_chunk
{
  my $length = shift;


  printf("%x\r\n",$length);
  system("./repeat.pl $length");
  printf("\r\n");

  system("./repeat.pl $length >>raw_chunk");
   
}


system("rm -rf raw_chunk");
make_chunk(16);
make_chunk(32);
make_chunk(64);
make_chunk(128);
make_chunk(256);
make_chunk(512);
make_chunk(1024);
make_chunk(2048);
make_chunk(4096);
make_chunk(8192);
make_chunk(0);






