# Better Breakout

A while ago, I decided to try my hand at creating a breakout clone in Rust. I'm
not sure how far I got, but I don't think I got too far. I've been becoming
increasingly interested in graphics programming over the course of the last
couple of months, and I've been following along with learnopengl.com's series
of tutorials.

One thing that I've been struggling with is that I can't seem to finish any
projects that I start. It's actually a big problem. Because of that, this
project will serve two purposes. First of all, it will be to improve my skills
at OpenGL and graphics programming, and second, I want to see this project
through from start to finish.

## Building and Running

### Building on Windows

Okay, so you chose suffering. Seriously, I don't get how people can willingly
suffer through software development on Windows. But, I digress.

In order to build this project, you need the following things:

* Microsoft Visual Studio 2022
* SDL2 development files
* SDL2 image development files

The first thing you should do if you haven't already is install Visual Studio.
Make sure you check "Desktop development with C++" in the installer, which will
install a bunch of useful things such as a C++ compiler and a bundled CMake
executable.

This would be a good time to make a new directory on your computer somewhere, this is where the zip archives that you download next should be stored.

Next, go to the [SDL2 releases page](https://github.com/libsdl-org/SDL/releases/)
on GitHub and install the latest release of SDL2. Make sure to grab the one that
says `SDL2-devel-{version}-VC.zip`. Then, go to the
[SDL2 image releases page](https://github.com/libsdl-org/SDL_image/releases) and
go through the same process. Make sure to download them both into that new directory.

Since we're using CMake to build the project, and CMake expects the SDL2
dependencies to be in specific places, you're going to have to set that up. This is where things get a bit trickier.

Unzip both of the archives. Then, create a new environment variable called CMAKE_PREFIX_PATH and set it to the following value:

```
C:\path\to\SDL2-{version};C:\path\to\SDL2-image-{version}
```

If you did all of that correctly, you shouldn't get any errors when building.

Now you can start up Visual Studio. Under the getting started menu, click Clone a Repository, and enter this GitHub repository. Once you open up the project, it should prompt you to let Visual Studio configure the project with CMake. Let it do its thing. At the top, look for the green play button and click on the dropdown to the right of it. Finally, select the option that says `breakout.exe (bin\breakout.exe)` and click on the play button. If you did everything correctly, you should see the game up and running!

Okay, so I don't often support Windows, so feel free to submit an issue if this seems too convoluted and/or hand-holdy. I'm not sure what Windows developers need or expect to see since I am not one.

### Building on Linux

Be grateful. Seriously. On most (if not all) Linux distributions, this process is only four commands long, and drop-dead simple.

In order to build this project, you need the following things:

* CMake
* Make or Ninja
* A C++ compiler
* SDL2 development files
* SDL2 image development files

If you're on Ubuntu, the command below should fetch all of those for you:

```sh
sudo apt install cmake ninja-build g++ libsdl2-dev libsdl2-image-dev
```

Once you've got all of the build dependencies installed, you can generate the
build files, compile, and run with the series of commands below:

```sh
cmake -S . -B build
ninja -C build
build/breakout
```

And you should be good! If you have any problems building, feel free to submit
an issue. If you're using a Linux distribution other than Ubuntu or a
derivative thereof, I trust that you are smart enough to figure out what
packages to install. Every mainline Linux distribution packages the required
software and development files.

## Roadmap

I'm taking a different approach with this project than I normally do. The
README.md file (the one you're reading) will contain a checklist of items that
I can implement one at a time, just to keep me organized and focused.

* [ ] Clean up main file
* [ ] Render multiple images
* [ ] Use arrow keys to move around an image

## License Information

Even though I'm following along with a tutorial series, I will not be copying
their code line by line, so the code is going to be my own, at least mostly.
I've decided to license my code under the public domain because I think that's
a chill thing to do. Feel free to use this code for whatever you wish, sell it,
modify it, claim that you wrote it, I genuinely don't care. See LICENSE for the
license text.
