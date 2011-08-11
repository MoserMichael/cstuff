#!/bin/bash

set -x
set -e

ROOT=/mnt/hgfs/mystuff/text


function up
{
  local to=$1
  local from=$2

  files=`ls $ROOT/$from/*.txt`

  rm -f $to/*
  for f in $files
  do
   b=`basename $f .txt`
   cp -rf $f $to/$b.textile
  done
}

cp -rf ../cstuff_bak/docs docs

sync

up all/blog/_posts blog
up all/projects/_posts projects
up all/ramblings/_posts ramble

git commit -a -m "Update site"
git push origin gh-pages
