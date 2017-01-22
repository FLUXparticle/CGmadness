# CGmadness
###### a Marble Madness clone

CGmadness is based on the classic game Marble Madness. It is running on OpenGL and uses current CG techniques like light maps, bump-mapping and reflection shader. It also has an editor where you can create your own levels.

## Hooks

If you plan to jump aroung in your clone of this repository, you should install this `post-checkout` hook in your `.git/hooks` directory.

```
#!/bin/sh

if [ "$(git-diff --name-status "$1" "$2" | egrep '\.cp?p?$' | sed 's/^..//;s/\.c.*//' | uniq -d)" ]
then
	echo "cleaning .deps/" >&2
	rm -rf .deps/ >/dev/null
fi
```

## Build

Build is tested on Linux and MacOS X. But should work on Windows, too, with [Cygwin](https://www.cygwin.com/) or [MinGW](http://www.mingw.org/).

#### Ubuntu 14.04 to 16.04

```
sudo apt-get install build-essential freeglut3-dev libglew-dev libglu1-mesa-dev
make
```

#### MacOS X

install [Homebrew](http://brew.sh/)

```
brew install gcc glew
make
```

