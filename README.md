# C++ Module Manager (cxxmm)

## What cxxmm is?
As the title shows, this program is a module manager for C++. But in fact, now it's only a tool to help you compile C++ module easily.

## How to install cxxmm?
- Preparations

  To install cxxmm, you need a C++ compiler (g++/clang++/...) and make.

  If you want to change your compile options, then edit makefile.

- Steps

  1.```cd cxxmm/```

  2.```make```

  3.```make install```
It probably fails, so we advise you to manually add $(INSTALL_PATH)/build to $PATH.

## How to use cxxmm?
For example, there is a C++ file ```main.cpp```, and it include a module ```test```. You just need to type ```cxxmm main.cpp``` then there will generate a executable file ```main```.
