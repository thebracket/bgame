#!/bin/bash
GAMEBASE=/Users/herbert/Development/github/bgame

echo "1a - Pulling latest source"
pushd $GAMEBASE/build
git pull

echo "1b - tagging the latest release"
VERSIONDATE=`date +"%Y_%m_%d"`
VERSION="${VERSIONDATE}_osx_nightly"
mv ../src/main/constants.hpp ../src/main/constants.hpp.backup
echo -e "#prama once\n\n#include <string>\n\nconst std::string VERSION=\"$VERSION\";\n\n" > ../src/main/constants.hpp

echo "1c - building latest version"
make -j8

echo "1d - restoring the previous version file to avoid conflicts"
mv ../src/main/constants.hpp.backup ../src/main/constants.hpp

now=`date +"%Y_%m_%d"`
tempfolder="/tmp/blackfuture_macosx_nightly_$now"
tarball="/tmp/blackfuture_macosx_nightly_${now}.tgz"
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

cp -R /Library/Frameworks/sfml-audio.framework sfml-audio.framework
cp -R /Library/Frameworks/sfml-graphics.framework sfml-graphics.framework
cp -R /Library/Frameworks/sfml-network.framework sfml-network.framework
cp -R /Library/Frameworks/sfml-system.framework sfml-system.framework
cp -R /Library/Frameworks/sfml-window.framework sfml-window.framework
cp /opt/local/lib/libz.1.dylib .
cp /System/Library/Frameworks/OpenGL.framework/Versions/A/OpenGL .
cp /opt/local/lib/libomp/libomp.dylib .
cp /usr/lib/libc++.1.dylib .
cp /usr/lib/libSystem.B.dylib .
cp $GAMEBASE/build/external/*.dylib .

echo "4 - stripping binaries"
strip $tempfolder/bgame

echo "5 - making tarball: ${tarball}"
tar cvfz $tarball *

popd
popd

#echo "6 - cleaning up folder"
rm -rf $tempfolder

echo "7 - publishing the tarball"
scp $tarball 172.16.10.193:/var/www/bfnightly

