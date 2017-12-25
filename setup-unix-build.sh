#!/bin/sh
GREEN='\033[0;32m'
NC='\033[0m' # No Color

echo -e "${GREEN}Updating External Libraries${NC}\n"
git submodule init
git submodule update
echo -e "${GREEN}Creating build directory${NC}\n"
mkdir build
cd build
echo -e "${GREEN}Setting up Makefiles${NC}\n"
cmake -g"Unix Makefiles" ../src || exit 1
echo -e "${GREEN}Making symbolic links${NC}\n"
ln -s ../world_defs .
ln -s ../assets .
ln -s ../rex .
echo -e "${GREEN}Making world folder${NC}\n"
mkdir world
echo -e "\n${GREEN}Ready - in the build folder, type "make" and then ./bgame${NC}"

