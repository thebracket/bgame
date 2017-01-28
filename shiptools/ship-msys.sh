#!/bin/bash
GAMEBASE=/home/herbert/github/bgame

echo "1a - Pulling latest source"
pushd $GAMEBASE/build
git pull

echo "1b - tagging the latest release"
VERSIONDATE=`date +"%Y_%m_%d"`
VERSION="${VERSIONDATE}_win64_nightly"
mv ../src/main/constants.hpp ../src/main/constants.hpp.backup
echo -e "#pragma once\n\n#include <string>\n\nconst std::string VERSION=\"$VERSION\";\n\n" > ../src/main/constants.hpp

echo "1c - building latest version"
make -j4

echo "1d - restoring the previous version file to avoid conflicts"
mv ../src/main/constants.hpp.backup ../src/main/constants.hpp

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
cp /mingw64/bin/libwinpthread-1.dll .
cp /mingw64/bin/libstdc++-6.dll .
cp /mingw64/bin/libgcc_s_seh-1.dll .
cp /mingw64/bin/libjpeg-8.dll .
cp /mingw64/bin/libfreetype-6.dll .
cp /mingw64/bin/libgomp-1.dll .
cp /mingw64/bin/lua51.dll .
cp /mingw64/bin/libbz2-1.dll .
cp /mingw64/bin/libharfbuzz-0.dll .
cp /mingw64/bin/zlib1.dll .
cp /mingw64/bin/libpng16-16.dll .
cp /mingw64/bin/libglib-2.0-0.dll .
cp /mingw64/bin/libgraphite2.dll .
cp /mingw64/bin/libintl-8.dll .
cp /mingw64/bin/libpcre-1.dll .
cp /mingw64/bin/libiconv-2.dll .

echo "4 - stripping binaries"
strip $tempfolder/bgame.exe

#echo "5 - making tarball: ${tarball}"
tar cvfz $tarball *

popd
popd

#echo "6 - cleaning up folder"
rm -rf $tempfolder

#echo "7 - publishing the tarball"
scp $tarball herbert@172.16.10.193:/var/www/bfnightly
rm $tarball
