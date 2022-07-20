OPT=-O2 -Wall
VER=-std=c++11
SHELLRC=~/.bashrc
PWD:=$(shell pwd)

compile: cxxmm

install: build/cxxmm
	@echo "export PATH=\"$(PWD)/build\":\$$PATH" >> $(SHELLRC)

cxxmm: src/main.cpp init
	$(CXX) $(OPT) $(VER) src/main.cpp -o build/cxxmm

test: build/cxxmm
	@cd ctest&&make&&make clean

init:
	@mkdir -p build/

clean:
	@rm -rf build/
