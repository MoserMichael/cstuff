#!/bin/sh

set -x 
set -e

#prefix=rungdb
#prefix="valgrind --leak-check=yes"
#--max-stackframe=1169834492" 

test_prog=$1
#test_prog=$BIN_ROOT_DIR/test/examplecoro

$prefix $test_prog tconfig.props &
srv_pid=$!
trap "kill $srv_pid" EXIT
sleep 10

curl -vv http://127.0.0.1:12321/headerservlet/
curl -vv http://127.0.0.1:12321/headerservlet/
curl -vv http://127.0.0.1:12321/headerservlet/
curl -vv http://127.0.0.1:12321/headerservlet/
curl -vv http://127.0.0.1:12321/headerservlet/

echo "*** all requests completed ***"









