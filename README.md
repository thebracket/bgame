# Black Future

An open-source game combining elements of Dwarf Fortress, Civilization, Warhammer, Douglass Adams, and more.

Please visit [our website](http://bracketproductions.com/index.php/blackfuture/) for more information, download links, etc.

*To build from source:*

1. Ensure that you have packages installed providing development versions of: Boost (system filesystem, iostreams), Lua 5.2, Zlib
2. Navigate to where you want to install the source. I use `~/Development/github` - but it can be whatever you want.
3. Clone the project: `git clone https://github.com/thebracket/bgame` (and wait a while)
4. Enter the project directory with `cd bgame`
5. Run the helper script to setup your build environment: `./setup-unix-build.sh`. This will create a "build" directory.
6. Enter the build directory, `cd build`
7. Build the game with `make`. You can use `make -j4` (replace the 4 with the number of cores in your system) to speed this step up.

Linux build tested on Ubuntu 15.10 (Wily), amd-64.

RLTK updates periodically, so it's a good idea to update it when you pull a new version to build.

*Unstable Builds*

I'm still getting the machinery together to post unstable builds. Right now, there is a repeatable - scripted - build setup, but it requires that I remember to run it on three different platforms! These are just starting to come together, so for now I can't make any promises that these will work at all... but you can download periodic builds [here](http://bfnightly.bracketproductions.com/)
