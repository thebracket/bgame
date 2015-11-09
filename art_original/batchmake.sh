#!/bin/bash
MAX_LAYER=16
MIN_LAYER=0
INFILE="16x16.xcf"
RESULT="../build/spritesheet.png"

mkdir tmp
for (( i=MIN_LAYER; i<=MAX_LAYER; i++ ))
do
	OUTFILE=tmp/img$i.png
	CONVERSION="convert $INFILE[$i] $OUTFILE"
	$CONVERSION
done
montage -background transparent -geometry 16x16\>+0+0 tmp/*.png $RESULT
rm tmp/*.png
rmdir tmp
convert Titles.xcf[1] ../build/loading_screen.png
convert Titles.xcf[0] ../build/main_menu.png
convert Menu\ Buttons.xcf[0] ../build/button1.png
convert Menu\ Buttons.xcf[1] ../build/button2.png
convert In\ Game.xcf[0] ../build/header.png

