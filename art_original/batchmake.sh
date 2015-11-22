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
convert Paused.xcf[0] ../build/paused.png
convert EmoteBubble.xcf[0] ../build/emote_bubble.png
convert PowerBar.xcf[0] ../build/power_bar_red.png
convert PowerBar.xcf[1] ../build/power_bar_green.png

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

convert 16x16.xcf[17] tmp/036.png
convert 16x16.xcf[18] tmp/037.png
convert 16x16.xcf[19] tmp/038.png
convert 16x16.xcf[20] tmp/039.png
convert 16x16.xcf[21] tmp/040.png
convert 16x16.xcf[22] tmp/041.png
convert 16x16.xcf[23] tmp/042.png
convert 16x16.xcf[24] tmp/043.png
convert 16x16.xcf[25] tmp/044.png
convert 16x16.xcf[26] tmp/045.png
convert 16x16.xcf[27] tmp/046.png
convert 16x16.xcf[28] tmp/047.png
convert 16x16.xcf[29] tmp/048.png
convert 16x16.xcf[30] tmp/049.png
convert 16x16.xcf[31] tmp/050.png
convert 16x16.xcf[32] tmp/051.png
convert 16x16.xcf[33] tmp/052.png
convert 16x16.xcf[34] tmp/053.png
convert 16x16.xcf[35] tmp/054.png
convert 16x16.xcf[36] tmp/055.png
convert 16x16.xcf[37] tmp/056.png
convert 16x16.xcf[38] tmp/057.png

# More people
convert PersonTemplate.xcf[1] tmp/058.png # Black Mask
convert PersonTemplate.xcf[2] tmp/059.png # Undressed Male
convert PersonTemplate.xcf[3] tmp/060.png # Undressed Female
convert PersonTemplate.xcf[4] tmp/061.png # Short Hair
convert PersonTemplate.xcf[5] tmp/062.png # Long Hair
convert PersonTemplate.xcf[6] tmp/063.png # Pigtails
convert PersonTemplate.xcf[7] tmp/064.png # Mohawk
convert PersonTemplate.xcf[8] tmp/065.png # Pointy Hair
convert PersonTemplate.xcf[9] tmp/066.png # Beard
convert PersonTemplate.xcf[10] tmp/067.png # Alice Hair
convert PersonTemplate.xcf[11] tmp/068.png # Trousers
convert PersonTemplate.xcf[12] tmp/069.png # Shirt
convert PersonTemplate.xcf[13] tmp/070.png # Shoes
convert PersonTemplate.xcf[14] tmp/071.png # Gloves
convert PersonTemplate.xcf[15] tmp/072.png # Short Skirt
convert PersonTemplate.xcf[16] tmp/073.png # Long skirt
convert PersonTemplate.xcf[17] tmp/074.png # Full Blouse
convert PersonTemplate.xcf[18] tmp/075.png # Small Blouse


montage -background transparent -geometry 16x16\>+0+0 tmp/*.png $RESULT
rm tmp/*
rmdir tmp

