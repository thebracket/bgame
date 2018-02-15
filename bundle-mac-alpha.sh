#!/bin/bash
BUTLER="~/Library/Application\ Support/itch/bin/butler"
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
#cp /System/Library/Frameworks/OpenGL.framework/Versions/A/OpenGL $FOLDER
cp /opt/local/lib/libboost_system-mt.dylib $FOLDER
cp /opt/local/lib/libboost_filesystem-mt.dylib $FOLDER
cp /opt/local/lib/libglfw.3.dylib $FOLDER
cp /opt/local/lib/liblua.dylib $FOLDER
cp /opt/local/lib/libomp/libomp.dylib $FOLDER
cp /usr/lib/libc++.1.dylib $FOLDER
cp /usr/lib/libSystem.B.dylib $FOLDER

echo "Fixing linkage on dylibs"
NF=$FOLDER/noxfutura
install_name_tool -change /opt/local/lib/libfmt.4.dylib @executable_path/libfmt.4.dylib $NF
install_name_tool -change /opt/local/lib/libz.1.dylib @executable_path/libz.1.dylib $NF
install_name_tool -change /usr/lib/libcurl.4.dylib @executable_path/libcurl.4.dylib $NF
install_name_tool -change /opt/local/lib/libGLEW.2.0.0.dylib @executable_path/libGLEW.2.0.0.dylib $NF
#install_name_tool -change /System/Library/Frameworks/OpenGL.framework/Versions/A/OpenGL @executable_path/OpenGL $NF
install_name_tool -change /opt/local/lib/libboost_system-mt.dylib @executable_path/libboost_system-mt.dylib $NF
install_name_tool -change /opt/local/lib/libboost_filesystem-mt.dylib @executable_path/libboost_filesystem-mt.dylib $NF
install_name_tool -change /opt/local/lib/libglfw.3.dylib @executable_path/libglfw.3.dylib $NF
install_name_tool -change /opt/local/lib/liblua.dylib @executable_path/liblua.dylib $NF
install_name_tool -change /opt/local/lib/libomp/libomp.dylib @executable_path/libomp.dylib $NF
install_name_tool -change /usr/lib/libc++.1.dylib @executable_path/libc++.1.dylib $NF
install_name_tool -change /usr/lib/libSystem.B.dylib @executable_path/libSystem.B.dylib $NF

echo "Butler time!"
echo $BUTLER
eval $BUTLER upgrade
UPLOAD_COMMAND="$BUTLER push $FOLDER thebracket/nox-futura:mac-pre-alpha"
echo $UPLOAD_COMMAND
eval $UPLOAD_COMMAND

