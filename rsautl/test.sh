#!/bin/bash

set -e
set -x

TEST_DATA="testdata"

if [ ! -d $TEST_DATA ]; then
  mkdir $TEST_DATA
fi  

dd if=/dev/urandom of=$TEST_DATA/100K.bin count=1 bs=100K

make_key_pair()
{
  pushd $TEST_DATA
  if [ ! -f private$1.pem ]; then
    openssl genrsa $1 >private$1.pem
  fi

  # create public key (4096)
  if [ ! -f public$1.pem ]; then
    openssl rsa -in private$1.pem -out public$1.pem -pubout
  fi
  popd
}  

test_it()
{
  # encrypt with 4096 public key
  time $BIN_ROOT_DIR/bin/rsaenc -k $TEST_DATA/public$1.pem -i $TEST_DATA/100K.bin -o $TEST_DATA/100K.enc 

  # decrypt with 4096 public key
  time $BIN_ROOT_DIR/bin/rsadec -k $TEST_DATA/private$1.pem -i $TEST_DATA/100K.enc -o $TEST_DATA/100K.bin2 

  diff -b $TEST_DATA/100K.bin $TEST_DATA/100K.bin2
}

make_key_pair 4096 
make_key_pair 2048 
make_key_pair 1024 

test_it 4096
test_it 2048
test_it 1024 

echo "*** test ok ***"
