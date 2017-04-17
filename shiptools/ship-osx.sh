#!/bin/bash
GAMEBASE=/Users/herbert/Development/github/bgame

echo "1a - Pulling latest source"
pushd $GAMEBASE/build
git pull

echo "1b - tagging the latest release"
VERSIONDATE=`date +"%Y_%m_%d"`
VERSION="${VERSIONDATE}_osx_nightly"
mv ../src/main/constants.hpp ../src/main/constants.hpp.backup
echo -e "#pragma once\n\n#include <string>\n\nconst std::string VERSION=\"$VERSION\";\n\n" > ../src/main/constants.hpp

echo "1c - building latest version"
make -j8

echo "1d - building os ax package and fixing paths"
make install

echo "1e - restoring the previous version file to avoid conflicts"
mv ../src/main/constants.hpp.backup ../src/main/constants.hpp

now=`date +"%Y_%m_%d"`
tempfolder="/tmp/blackfuture_macosx_nightly_$now"
tarball="/tmp/blackfuture_macosx_nightly_${now}.tgz"
echo "2 - creating $tempfolder and populating it with game data"
mkdir -p $tempfolder
mkdir -p ${tempfolder}/bf.app
APP=${tempfolder}/bf.app
APPCONTENT=${APP}/Contents/MacOS
cp -R ${GAMEBASE}/build/bf.app/* ${APP}
mkdir -p ${APPCONTENT}/assets
mkdir -p ${APPCONTENT}/rex
mkdir -p ${APPCONTENT}/world
mkdir -p ${APPCONTENT}/world_defs
pushd ${APPCONTENT}

cp $GAMEBASE/assets/*.ttf assets
cp $GAMEBASE/assets/terminal8x8.png assets
cp $GAMEBASE/assets/terminal32x32.png assets
cp $GAMEBASE/assets/terminal16x16.png assets
cp $GAMEBASE/assets/VGA8x16.png assets
cp $GAMEBASE/assets/Nagidal24x24shade.png assets
cp $GAMEBASE/assets/Hack_square_64x64.png assets
cp $GAMEBASE/assets/fonts.txt assets
cp $GAMEBASE/assets/background_image.* assets
cp $GAMEBASE/assets/gamelogo.png assets

cp $GAMEBASE/world_defs/* world_defs
cp $GAMEBASE/rex/* rex

echo "4 - stripping binaries"
strip ${APPCONTENT}/bf

echo "5 - making tarball: ${tarball}"
pushd ${tempfolder}
tar cvfz $tarball *
popd

popd
popd

echo "6 - cleaning up folder"
rm -rf $tempfolder

echo "7 - publishing the tarball"
scp $tarball 172.16.10.193:/var/www/bfnightly
rm $tarball
