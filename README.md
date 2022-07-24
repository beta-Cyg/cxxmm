# C++ Module Manager (cxxmm)

## What cxxmm is?
As the title shows, this program is a module manager for C++. But in fact, now it's only a tool to help you compile C++ module easily.

[2022.07.22] now it supports some simple operations. (pack, unpack, remove)

## How to install cxxmm?
- Requests

	To install cxxmm, you need a C++ compiler(g++/clang++/...) and make.

	To use cxxmm, you need python3.

- Preparations

	If you want to change your compile options, then edit makefile.

- Steps

	1.```cd cxxmm/```

	2.```make```

	3.```make install```
	It probably fails, so we advise you to manually add $(INSTALL_PATH)/build to $PATH.

## How to use cxxmm?
- Compile
	
	```sh
	cxxmm compile [file1] [file2] ...
	```
	e.g.
	```sh
	cxxmm compile main.cpp
	```

- Pack
	
	```sh
	cxxmm pack [directory1] [directory2] ...
	```
	e.g.
	```sh
	cxxmm pack test/
	```
	cxxmm will generate a package ```test.tar```

- Unpack
	```sh
	cxxmm unpack [package1] [package2] ...
	```
	e.g.
	```sh
	cxxmm unpack test
	```
	cxxmm will unpack the package ```test.tar``` and generate a directory ```test/```

- Remove
	```sh
	cxxmm remove [package1] [package2] ...
	```
	e.g.
	```sh
	cxxmm remove test
	```
	cxxmm will remove the package ```test.tar```

## More Info
- package format of cxxmm is tar
- the pack module of cxxmm depends on python3

## Issues
- cxxmm cannot print error information when pack.py has problems.

# Cygnus Package Manager (cpm)
