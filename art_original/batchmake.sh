#!/bin/bash


#MAX_LAYER=16
#MIN_LAYER=0
#INFILE="16x16.xcf"
RESULT="../build/spritesheet.png"

#mkdir tmp
#for (( i=MIN_LAYER; i<=MAX_LAYER; i++ ))
#do
#	OUTFILE=tmp/img$i.png
#	CONVERSION="convert $INFILE[$i] $OUTFILE"
#	$CONVERSION
#done
#montage -background transparent -geometry 16x16\>+0+0 tmp/*.png $RESULT
#rm tmp/*.png
#rmdir tmp

echo "Converting Simple Graphics"
convert Titles.xcf[1] ../build/loading_screen.png
convert Titles.xcf[0] ../build/main_menu.png
convert Menu\ Buttons.xcf[0] ../build/button1.png
convert Menu\ Buttons.xcf[1] ../build/button2.png
convert In\ Game.xcf[0] ../build/header.png

mkdir tmp
convert 16x16.xcf[0]  tmp/001.png
convert 16x16.xcf[1]  tmp/002.png
convert 16x16.xcf[2]  tmp/003.png
convert 16x16.xcf[3]  tmp/004.png
convert 16x16.xcf[4]  tmp/005.png
convert 16x16.xcf[5]  tmp/006.png
convert 16x16.xcf[6]  tmp/007.png
convert 16x16.xcf[7]  tmp/008.png
convert 16x16.xcf[8]  tmp/009.png
convert 16x16.xcf[9]  tmp/010.png
convert 16x16.xcf[10] tmp/011.png
convert 16x16.xcf[11] tmp/012.png
convert 16x16.xcf[12] tmp/013.png
convert 16x16.xcf[13] tmp/014.png
convert 16x16.xcf[14] tmp/015.png
convert 16x16.xcf[15] tmp/016.png
convert 16x16.xcf[16] tmp/017.png

convert Structures.xcf[0] tmp/018.png
convert Structures.xcf[1] tmp/019.png
convert Structures.xcf[2] tmp/020.png
convert Structures.xcf[3] tmp/021.png
convert Structures.xcf[4] tmp/022.png
convert Structures.xcf[5] tmp/023.png
convert Structures.xcf[6] tmp/024.png
convert Structures.xcf[7] tmp/025.png
convert Structures.xcf[8] tmp/026.png
convert Structures.xcf[9] tmp/027.png
convert Structures.xcf[10] tmp/028.png
convert Structures.xcf[11] tmp/029.png
convert Structures.xcf[12] tmp/030.png
convert Structures.xcf[13] tmp/031.png
convert Structures.xcf[14] tmp/032.png
convert Structures.xcf[15] tmp/033.png
convert Structures.xcf[16] tmp/034.png

convert PersonTemplate.xcf[0] tmp/035.png

montage -background transparent -geometry 16x16\>+0+0 tmp/*.png $RESULT
rm tmp/*
rmdir tmp

