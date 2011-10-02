#!/bin/bash

set -x
set -e

cp -rf ../cstuff_bak/docs/html docs

git commit -a -m "Update source documentation"
git push origin gh-pages

