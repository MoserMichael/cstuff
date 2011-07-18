#!/bin/sh

set -x

EXE_FILE=$1 

$EXE_FILE 

../../scripts/readerrors.pl errors.log | tee errors.result
