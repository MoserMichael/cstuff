#!/bin/sh

set -x
set -e

graph-easy  --as_dot  --input=pars.graph  --output=pars.dot 
dot pars.dot -Tps -opars.ps
