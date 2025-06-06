# Kmap
## About

`kmap` is my personal japanese dictionary that I've made a while back (somewhere between 2020-2021, I think?).
The code isn't particularly great, but it works. The releases are self-contained "portable" programs that you can run from anywhere, provided that your pc supports opengl 4.2.

## Compiling
This program compiles a bunch of dictionaries together into a common database that is then included into the main executable and used from there.
The dictionaries are in dicts/ folder, but you can try and use the newer versions (the parser is very picky though!)

To make the database by itself, run `make build/db.h`.
Alternatively, just running make should build the linux and windows debug builds.
To make the release build, run `make build/release` for the linux version, or `make build/release.exe` for the windows version.

### Linux
You're going to need the development versions of:
- Xlib
- OpenGL

And also make and sed as compilation dependencies

### Windows
There's nothing preventing this from being compilable on windows (and it does work with a mingw cross-compiler on linux),
but the toolchain is not set up for compiling on windows since I never do that.
You can use build/win\_debug.exe make target as a starting point if you want to do that. (using mingw or cygwin would probably be the least painful way of going about that)

## Running

You should run the debug builds from the same directory you've compiled them from, since the executable is split into theloader and dll.
The release is self-contained so you should be able to run it from anywhere.

## Usage

Type in text to search (IME should work also).
Use right mouse button to select things, and left button to move around.
Once you've selected a character, you can press `tab` or `shift+tab` to explore other words that have that same character.
Navigate around search result via `F4` and `F5`

## Credits
(I've never written down these when I was making kmap originally, sorry if I missed something!)

- Frequency list: http://corpus.leeds.ac.uk/list.html
- JMdict: https://www.edrdg.org/jmdict/j\_jmdict.html
- Jyouou stroke edit distance info for finding similar kanji: (forgot where the link came from)
- Kanjidic: https://www.edrdg.org/wiki/index.php/KANJIDIC\_Project
- Kradfile and radkfile: https://www.edrdg.org/krad/kradinf.html
- stb\_truetype.h: https://github.com/nothings/stb
