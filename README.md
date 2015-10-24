# bgame - Black Future

__Black Future__ will, hopefully, one day be a fun game to play. For now, it's little more than a demo of some concepts:
* A __device independent back-end__, providing a virtual terminal that can render to SDL2, NCurses and an ANSII escape-code renderer.
* A __GUI framework__, using shallow inheritance and some layout features, with a menu and layout to test it.
* A __Console__, supporting tags to provide color. It doesn't line-wrap correctly yet.
* A simple __entity component system__, with the beginnings of some game elements to use it.
* A __world generator__, which produces (and saves) a large world. The generator starts with a height-map, and then divides the world into water/not-water. Marching squares then adds contours, and marks beach areas. Tiles are then generated according to a temperature map/altitude system.
* A __lighting system__, which shades ground tiles based upon time of day. The demo right now quickly runs through day/night to show this off.

![Screenshot](https://raw.githubusercontent.com/thebracket/bgame/master/assets/hyper-game-demo.gif "Demo of lighting and random movement")

## To build this:
It requires: SDL2, SD2 Image, and NCurses. I've tested on Linux and OS X. On Linux, I'm using g++5 - since I'd really miss the C++14 allocator lists. It's a pretty standard cmake, so:

### To setup the first time:
````
mkdir build
cd build
cmake -g "Unix Makefiles" ..
make
./bgame
````

### To build after that
````
cd build
mkdir world
make
./bgame
````
