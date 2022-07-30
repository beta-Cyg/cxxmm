OPT=-O2 -Wall -Wextra
SHARED=-fPIC -shared
VER=-std=c++11
SHELLRC=~/.bashrc
LINK_PATH=lib/
PWD:=$(shell pwd)

compile: ini main cpm

install: bin/cxxmm shell/pack.py
	@echo "export PATH=\"$(PWD)/bin\":\"$(PWD)/shell\":$$PATH" >> $(SHELLRC)
	@echo "export LD_LIBRARY_PATH=\"$(PWD)/lib\"$$PATH" >> $(SHELLRC)

cpm: client server

client: src/client.cpp log
	$(CXX) $(OPT) $(VER) src/client.cpp -o bin/cpm -L$(LINK_PATH) -lpthread -llog -lsplit

server: src/server.cpp
	$(CXX) $(OPT) $(VER) src/server.cpp -o bin/server -L$(LINK_PATH) -lpthread

main: library src/main.cpp
	$(CXX) $(OPT) $(VER) src/main.cpp -o bin/cxxmm -L$(LINK_PATH) -lmake

library: make
	rm -rf tmp/

make: result split tree src/make.h src/make.cpp
	$(CXX) $(OPT) $(VER) -c src/make.cpp -o tmp/make.o
	ar rcs lib/libmake.a tmp/make.o tmp/result.o tmp/split.o tmp/tree.o tmp/log.o
	$(CXX) $(SHARED) src/make.cpp src/make/result.cpp src/make/split.cpp src/make/tree.cpp src/make/log.cpp -o lib/libmake.so

result: log src/make/result.h src/make/result.cpp
	$(CXX) $(OPT) $(VER) -c src/make/result.cpp -o tmp/result.o
	ar rcs lib/libresult.a tmp/result.o tmp/log.o
	$(CXX) $(SHARED) src/make/result.cpp src/make/log.cpp -o lib/libresult.so

split: src/make/split.h src/make/split.cpp
	$(CXX) $(OPT) $(VER) -c src/make/split.cpp -o tmp/split.o
	ar rcs lib/libsplit.a tmp/split.o
	$(CXX) $(SHARED) src/make/split.cpp -o lib/libsplit.so

tree: src/make/tree.h src/make/tree.cpp
	$(CXX) $(OPT) $(VER) -c src/make/tree.cpp -o tmp/tree.o
	ar rcs lib/libtree.a tmp/tree.o
	$(CXX) $(SHARED) src/make/tree.cpp -o lib/libtree.so

log: src/make/log.h src/make/log.cpp
	$(CXX) $(OPT) $(VER) -c src/make/log.cpp -o tmp/log.o
	ar rcs lib/liblog.a tmp/log.o
	$(CXX) $(SHARED) src/make/log.cpp -o lib/liblog.so

test: bin/cxxmm shell/pack.py
	@cd ctest&&make&&make clean

ini:
	@mkdir -p bin/ lib/ tmp/

clean:
	@rm -rf bin/ lib/ tmp/
