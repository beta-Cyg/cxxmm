OPT=-O2 -Wall
VER=-std=c++11
SHELLRC=~/.bashrc
LINK_PATH=lib/
PWD:=$(shell pwd)

compile: ini main cpm

install: bin/cxxmm shell/pack.py
	@echo "export PATH=\"$(PWD)/build\":\"$(PWD)/shell\":$$PATH" >> $(SHELLRC)

cpm: client server

client: src/client.cpp log
	$(CXX) $(OPT) $(VER) src/client.cpp -o bin/cpm -L$(LINK_PATH) -lpthread -llog

server: src/server.cpp
	$(CXX) $(OPT) $(VER) src/server.cpp -o bin/server -L$(LINK_PATH) -lpthread

main: make src/main.cpp
	$(CXX) $(OPT) $(VER) src/main.cpp -o bin/cxxmm -L$(LINK_PATH) -lmake

make: result split tree src/make.h src/make.cpp
	$(CXX) $(OPT) $(VER) -c src/make.cpp -o lib/make.o
	ar rcs lib/libmake.a lib/make.o lib/result.o lib/split.o lib/tree.o lib/log.o

result: log src/make/result.h src/make/result.cpp
	$(CXX) $(OPT) $(VER) -c src/make/result.cpp -o lib/result.o
	ar rcs lib/libresult.a lib/result.o lib/log.o

split: src/make/split.h src/make/split.cpp
	$(CXX) $(OPT) $(VER) -c src/make/split.cpp -o lib/split.o
	ar rcs lib/libsplit.a lib/split.o

tree: src/make/tree.h src/make/tree.cpp
	$(CXX) $(OPT) $(VER) -c src/make/tree.cpp -o lib/tree.o
	ar rcs lib/libtree.o lib/tree.o

log: src/make/log.h src/make/log.cpp
	$(CXX) $(OPT) $(VER) -c src/make/log.cpp -o lib/log.o
	ar rcs lib/liblog.a lib/log.o

test: bin/cxxmm shell/pack.py
	@cd ctest&&make&&make clean
	@cxxmm pack ctest
	@cxxmm unpack ctest
	@cxxmm remove ctest

ini:
	@mkdir -p bin/ lib/

clean:
	@rm -rf bin/ lib/
