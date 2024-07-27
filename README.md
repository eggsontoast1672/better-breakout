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

Currently, this project only supports building on Linux. There may be plans to
add Windows support in the future, but probably not for a while. Let's just say
that Windows and MacOS builds are pretty far down on the priority list.

In order to build this project, you need the following things:

* CMake
* Make or Ninja
* A C++ compiler
* SDL2 development files

If you're on Ubuntu, the command below should fetch all of those for you:

```sh
sudo apt install cmake ninja-build g++ libsdl2-dev
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

* [X] Set up hello world
* [X] Get a window up
* [X] Add error handling
* [X] Create an OpenGL context
* [X] Display a triangle
* [X] Display an image
* [ ] Flip image right-side-up

## License Information

Even though I'm following along with a tutorial series, I will not be copying
their code line by line, so the code is going to be my own, at least mostly.
I've decided to license my code under the public domain because I think that's
a chill thing to do. Feel free to use this code for whatever you wish, sell it,
modify it, claim that you wrote it, I genuinely don't care. See LICENSE for the
license text.
