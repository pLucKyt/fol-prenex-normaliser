# First order logic to prenex normal form

Reads a first order logic formula and converts it into prenex normal form, where all
the quantifiers are pulled out to the front. Written in C++. This one is older than
the rest: it was my high school final paper (maturski rad).

The logic symbols are the real Unicode ones, so the whole program works on wide
characters and wide strings rather than bytes.

## Building

One file, no dependencies beyond the standard library. It uses `_setmode` and
`<io.h>` to put the console into UTF-16 mode, so it builds on Windows (MinGW or
MSVC), not on Linux as it stands.

    g++ main.cpp -o normaliser
