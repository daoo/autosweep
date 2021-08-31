# Autosweep

[![License](https://img.shields.io/github/license/daoo/autodesk.svg)](https://github.com/daoo/autodesk/blob/master/LICENSE.txt)

Automatically play Minesweeper, written in C++. Supports https://minesweeperonline.com/. Example:

![Example Play](example.gif)

## Building

Required libraries: OpenCV and X11.

Uses meson and ninja for building:

    meson builddir
    ninja -C builddir
