#!/bin/bash
MAX_LAYER=9
MIN_LAYER=0
INFILE="32x32.xcf"

mkdir tmp
for (( i=MIN_LAYER; i<=MAX_LAYER; i++ ))
do
	OUTFILE=tmp/img$i.png
	CONVERSION="convert $INFILE[$i] $OUTFILE"
	$CONVERSION
done
montage -background transparent -geometry 32x32\>+0+0  tmp/*.png spritesheet.png
rm tmp/*.png
rmdir tmp

