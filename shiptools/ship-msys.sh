#!/bin/bash
GAMEBASE=/home/herbert/github/bgame

echo "1 - Pulling latest source and building"
pushd $GAMEBASE/build
git pull
make -j4

now=`date +"%Y_%m_%d"`
tempfolder="/tmp/blackfuture_win64_nightly_$now"
tarball="/tmp/blackfuture_win64_nightly_${now}.tgz"
echo "2 - creating $tempfolder and populating it with game data"
mkdir -p $tempfolder
mkdir -p $tempfolder/assets
mkdir -p $tempfolder/rex
mkdir -p $tempfolder/world
mkdir -p $tempfolder/world_defs
pushd $tempfolder
cp $GAMEBASE/build/bgame .

cp $GAMEBASE/assets/*.ttf assets
cp $GAMEBASE/assets/terminal8x8.png assets
cp $GAMEBASE/assets/terminal32x32.png assets
cp $GAMEBASE/assets/terminal16x16.png assets
cp $GAMEBASE/assets/VGA8x16.png assets
cp $GAMEBASE/assets/Nagidal24x24shade.png assets
cp $GAMEBASE/assets/Hack_square_64x64.png assets
cp $GAMEBASE/assets/fonts.txt assets
cp $GAMEBASE/assets/background_image.png assets
cp $GAMEBASE/assets/gamelogo.png assets

cp $GAMEBASE/world_defs/* world_defs
cp $GAMEBASE/rex/* rex

echo "5 - copying dependencies"
cp /mingw64/bin/sfml*.dll .

echo "4 - stripping binaries"
strip $tempfolder/bgame.exe

#echo "5 - making tarball: ${tarball}"
#tar cvfz $tarball *

popd
popd

#echo "6 - cleaning up folder"
#rm -rf $tempfolder

#echo "7 - publishing the tarball"
#mv $tarball /var/www/bfnightly

