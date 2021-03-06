#!/bin/bash

set -x
set -e

mkdir -p ../docs/html/cutils
doxygen  oct-cutils.doxygen

mkdir -p ../docs/html/tutils
doxygen  oct-tutils.doxygen

mkdir -p ../docs/html/nutils
doxygen  oct-nutils.doxygen

mkdir -p ../docs/html/butils
doxygen  oct-butils.doxygen

mkdir -p ../docs/html/corothread
doxygen  oct-corothread.doxygen

mkdir -p ../docs/html/evcorothread
doxygen  oct-evcorothread.doxygen

mkdir -p ../docs/html/hutils
doxygen  oct-hutils.doxygen

mkdir -p ../docs/html/webby
doxygen  oct-webby.doxygen

mkdir -p ../docs/html/vtest
doxygen  oct-vtest.doxygen



