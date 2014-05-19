nanovg_example
==============

My examples using NanoVG

How to build
------------

### OS X

Install libraries:

```
% brew install glew glfw3 sdl2
```

Run `update_ninja_file.sh` to create build.ninja.
`update_ninja_file.sh` requires [Ruby](http://www.ruby-lang.org/) and `erb` command:

```
% ./update_ninja_file.sh
```

Use [Ninja](https://github.com/martine/ninja) to build.

```
% ninja
```

### Windows

Get and unzip files:

- glew: https://sourceforge.net/projects/glew/files/glew/1.10.0/glew-1.10.0-win32.zip/download
- glfw: http://sourceforge.net/projects/glfw/files/glfw/3.0.4/glfw-3.0.4.zip/download
- SDL: http://libsdl.org/release/SDL2-devel-2.0.3-VC.zip

Use `vse2013/nanovg_example-vse2013.sln` with Visual Studio Express 2013.

Font
----

`nanovg_example_glfw` and `nanovg_example_sdl` requires `mplus-1p-regular.ttf` to draw the text.
You can get this font from [M+ FONTS](http://mplus-fonts.sourceforge.jp/)
