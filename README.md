# bgame - Bert's Game

A github repository for work on my roguelike. Right now, it's little more than a graphics framework:
the engine/ folder contains the framework required for SDL2 (using a terminal texture, similar to libtcod) and ncurses as back-ends. There's also the skeleton of a stack-based mode system.

Eventually, this will become a useful library and hopefully also a game!

Right now, it's listed as GPL2 because I like it. If anyone would like to use this, I'm happy to talk about that (and I'm the only contributor, so it's mine to re-license).

## To build this:
It requires: SDL2, SD2 Image, and NCurses. I've tested on Linux and OS X. On Linux, I'm using g++5 - since I'd really miss the C++14 allocator lists. It's a pretty standard cmake, so:

### To setup the first time:
mkdir build

cd build

cmake -g "Unix Makefiles" ..

make

./bgame

### To build after that
cd build

make

./bgame
