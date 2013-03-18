#!/bin/bash

OUT="$1"
[ -n "$OUT" ] || OUT="vsync.avi"

TMP=".build.tmp"

rm -rf ${TMP}
mkdir ${TMP}
convert -size 640x480 -depth 24 canvas:black png24:${TMP}/black.png
convert -size 640x480 -depth 24 canvas:white png24:${TMP}/white.png

mkdir ${TMP}/anim

for ((a=0; $a < 1000; a=$a+2)); do
	ln -s ../black.png ${TMP}/anim/$a.png
done

for ((a=1; $a < 1000; a=$a+2)); do
	ln -s ../white.png ${TMP}/anim/$a.png
done

mencoder "mf://${TMP}/anim/*.png" -v -vf-clr -mf fps=60 -o "${OUT}" -ovc lavc
exitcode=$?
rm -rf ${TMP}

exit ${exitcode}
