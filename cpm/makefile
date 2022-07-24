OPT=-O2 -lpthread

compile: ini server client

server: src/server.cpp
	$(CXX) $(OPT) src/server.cpp -o bin/server

client: src/client.cpp
	$(CXX) $(OPT) src/client.cpp -o bin/client

ini:
	@mkdir -p bin/

clean:
	@rm -rf bin/
