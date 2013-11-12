#!/bin/sh

set -e
set -x

dd if=/dev/urandom of=100K.bin count=1 bs=100K

make_key_pair()
{
  if [ ! -f private$1.pem ]; then
    openssl genrsa $1 >private$1.pem
  fi

  # create public key (4096)
  if [ ! -f public$1.pem ]; then
    openssl rsa -in private$1.pem -out public$1.pem -pubout
  fi
}  

test_it()
{
  # encrypt with 4096 public key
  time $BIN_ROOT_DIR/bin/rsaenc -k public$1.pem -i 100K.bin -o 100K.enc -v

  # decrypt with 4096 public key
  time $BIN_ROOT_DIR/bin/rsadec -k private$1.pem -i 100K.enc -o 100K.bin2 -v

  diff -b 100K.bin 100K.bin2
}

make_key_pair 4096 
make_key_pair 2048 
make_key_pair 1024 

test_it 4096
test_it 2048
test_it 1024 
