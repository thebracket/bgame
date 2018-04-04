# Nox Futura (formerly Black Future)

An open-source game combining elements of Dwarf Fortress, Civilization, Warhammer, Douglas Adams, and more. It's very much in development, so don't expect miracles - or a finished experience - yet!

Please visit [our website](http://bracketproductions.com/index.php/blackfuture/) 
for more information, download links, etc.

Note that OS X isn't supported at the present time.

## Contributions

I hope one day to release this on various platforms, so I'm not merging in PRs. It's open source, because I firmly believe that users should be able to see what they are running - but I'm not really looking for collaboration at this time. Sorry! If you do want to submit a change, I'll need to talk to you about license assignment - I need to be able to release re-licensed builds (and possibly one day make money off them), and I don't want to get into the nightmare of "I made $5, now I have to get $0.10 to contributor X, $0.05 to contributor Y, etc.".

As the game gets more mature, I'm hoping that providing the source will make life easier for modders, extenders and fans.

If you see something you like, feel free to use it (subject to the GPL license). I'm all in favor of assisting others.

## Build Instructions by Platform

*Unstable Builds*

I'm still getting the machinery together to post unstable builds. Right now, there is a repeatable - scripted - build setup, but it requires that I remember to run it on three different platforms! These are just starting to come together, so for now I can't make any promises that these will work at all... but you can download periodic builds [from itch.io](https://thebracket.itch.io/nox-futura)

*Windows Builds*

Install packages (see below), open in VS2017 (there's a solution file included) and you should be able to build it.

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
* mingw64/mingw-w64-x86_64-fmt
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

You can now run `./noxfutura.exe` to run the game. Note that `ln -s` on MSYS2 appears to just make a copy, so you may need to re-copy after the world updates. I have no idea why they would do this, NTFS supports links just fine!

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
