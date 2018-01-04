# Nox Futura (formerly Black Future)

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

*Windows Builds*

Edit the CMakeSettings.json to point to your vcpkg installation, install packages (see below), open in VS2017 and you should be able to build it.

Packages: boost (which pulls in zlib for you), curl, cereal, lua, glfw3, glew, glm. Sadly, this will take a while; on my Core i7 with 12 Gb RAM it took over an hour. The good news is that you only have to do it the once.

After that, you need to use the base as your working directory - so it can see game_assets and world_defs.x

There is an included Visual Studio project to make this easier.

*Windows builds with MSYS2 - Unix compatibility*

If you are using MSYS2 to build, you need to install (with `pacman -S`):

* mingw64/mingw-w64-x86_64-libcurl-devel
* mingw64/mingw-w64-x86_64-glew
* mingw64/mingw-w64-x86_64-glfw
* mingw64/mingw-w64-x86_64-boost
* mingw64/mingw-w64-x86_64-cereal
* mingw64/mingw-w64-x86_64-glm
* mingw64/mingw-w64-x86_64-libc++
* mingw64/mingw-w64-x86_64-ccache (to speed up compilation)
* mingw64/mingw-w64-x86_64-cotire (to speed up compilation)
* tar (yes, that really isn't included by default...)

The Lua libraries bundled with MSYS2 do not work (at least for me), so I bundled Lua in the source directory.

Change directory to wherever you checked out bgame, and:

* `mkdir build`
* `cd build`
* `cmake -G"Unix Makefiles" ../src`
* `make -j4`
* `cp lua/liblua.dll .`
* `ln -s ../world_defs .`
* `ln -s ../game_assets .`

You can now run `./noxfutura.exe` to run the game.

This basic setup also worked with CLion on my machine.

*Linux Builds*

This is on the latest Linux Mint (18.3 at the time of writing). For some reason, it ships with old developer tools (disappointing!). This should fix that:

  sudo add-apt-repository ppa:ubuntu-toolchain-r/test
  sudo apt-get update
  sudo apt-get install gcc-7 g++-7

You need to install the following packages (I did this on Ubuntu; other distros may vary): `build-essential`, `git`, `cmake`, `extra-cmake-modules`, `libcurl4-openssl-dev`, `libglew-dev`, `libglfw3-dev`, `libglm-dev`, `libboost-dev`, `libboost-system-dev`, `libboost-filesystem-dev`, `zlibc`, `libcereal-dev`, `libncurses5-dev`, `libreadline-dev`, `liblua5.3-dev`, `libglew-dev`

To build it:

* Clone the github repo into a folder. `git clone https://github.com/thebracket/bgame`.
* Enter the bgame folder, `cd bgame`.
* Make a build folder, `mkdir build`, and enter it with `cd build`.
* Tell CMake to create a build structure. `cmake -G"Unix Makefiles" ../src`.
* Build the app with `make -j4` (replace the 4 with how many CPUs you would like to use)
* `ln -s ../world_defs .`
* `ln -s ../game_assets .`
* You can now launch the game with `./noxfutura`
