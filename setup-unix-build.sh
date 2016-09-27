#!/bin/bash
GREEN='\033[0;32m'
NC='\033[0m' # No Color

git submodule init
echo -e "${GREEN}Creating build directory${NC}\n"
mkdir build
echo -e "${GREEN}Updating RLTK${NC}\n"
pushd src/rltk
git pull https://github.com/thebracket/rltk
popd
cd build
echo -e "${GREEN}Setting up Makefiles${NC}\n"
cmake -g"Unix Makefiles" ../src
echo -e "${GREEN}Making symbolic links${NC}\n"
ln -s ../world_defs .
ln -s ../assets .
ln -s ../rex .
echo -e "${GREEN}Making world folder${NC}\n"
mkdir world
echo -e "\n${GREEN}Ready - in the build folder, type "make" and then ./bgame${NC}"

