#!/bin/bash
BUTLER=~/Library/Application\ Support/itch/bin/butler
DATE=`date +%Y-%m-%d`
FOLDER=/opt/bgame$DATE
#echo $FOLDER

echo "Making folders in $FOLDER"
mkdir $FOLDER
mkdir $FOLDER/game_assets
mkdir $FOLDER/world_defs

echo "Assembling assets"
cp -R world_defs/* $FOLDER/world_defs
cp -R game_assets/* $FOLDER/game_assets
cp build/noxfutura $FOLDER

echo "Stripping binary"
strip $FOLDER/noxfutura

echo "Copying dylibs"
cp /opt/local/lib/libfmt.4.dylib $FOLDER
cp /opt/local/lib/libz.1.dylib $FOLDER
cp /usr/lib/libcurl.4.dylib $FOLDER
cp /opt/local/lib/libGLEW.2.0.0.dylib $FOLDER
cp /System/Library/Frameworks/OpenGL.framework/Versions/A/OpenGL $FOLDER
cp /opt/local/lib/libboost_system-mt.dylib
cp /opt/local/lib/libboost_filesystem-mt.dylib $FOLDER
cp /opt/local/lib/libglfw.3.dylib $FOLDER
cp /opt/local/lib/liblua.dylib $FOLDER
cp /opt/local/lib/libomp/libomp.dylib $FOLDER
cp /usr/lib/libc++.1.dylib $FOLDER
cp /usr/lib/libSystem.B.dylib $FOLDER

