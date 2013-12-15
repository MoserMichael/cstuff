#!/bin/sh

set -x
set -e

SRC=strdup.c
OUT=strdup

gcc ${SRC}  -o ${OUT}

objdump -T ${OUT}

gcc ${SRC} -o ${OUT} -O3

objdump -T ${OUT}

gcc ${SRC} -o ${OUT} -O3 -D_FORTIFY_SOURCE=1

objdump -T ${OUT}

gcc ${SRC} -o ${OUT} -O3 -D_FORTIFY_SOURCE=2

objdump -T ${OUT}

gcc ${SRC} -o ${OUT} -O3 -D__OPTIMIZE__

objdump -T ${OUT}
