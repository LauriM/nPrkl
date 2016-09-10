# nPrkl
Cross platform terminal user interface library, similar to ncurses and termbox.

Are you tired of ncurses color limitations? Are you sick of PDCurses? Do you find Termbox good but don't like that its not cross platform? Do you just want to include one header and be done with it?

If you say yes, nPrkl might be the choice of a library for you.

Windows builds: [![Build status Windows](https://ci.appveyor.com/api/projects/status/xfaopqq2k56t6fyx?svg=true)](https://ci.appveyor.com/project/LauriM/nprkl)

Linux builds: [![Build Status Linux](https://travis-ci.org/LauriM/nPrkl.svg?branch=master)](https://travis-ci.org/LauriM/nPrkl)

![Perkele](http://i.imgur.com/mVU1iqd.png)

Features:

* single header file
* real cross platform support

Usage:

* include nPrkl.h where you want to use it
* in one compilation unit define NPRKL_IMPLEMENTATION before the include

Planned features:

* truecolor support
* input
