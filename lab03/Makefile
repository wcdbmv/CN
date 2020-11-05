CXXFLAGS = -std=c++2a -Wall -Werror -Wextra
LDFLAGS = -pthread

all: client server

client: client.o utils.o
	$(CXX) $(LDFLAGS) $^ -o client.out

server: server.o statistics.o http_server.o utils.o
	$(CXX) $(LDFLAGS) $^ -o server.out

statistics.o: statistics.hpp
utils.o: utils.hpp
http_server.o: http_server.hpp const.hpp statistics.hpp utils.hpp
client.o: const.hpp utils.hpp
server.o: const.hpp http_server.hpp utils.hpp

clean:
	rm *.o *.out
