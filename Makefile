TARGETS = server.out client.out
server_OBJS = distance.o knn.o CLI.o commands.o defaultIO.o server.o
client_OBJS = distance.o knn.o client.o defaultIO.o
SOURCES = distance.cpp knn.cpp server.cpp client.cpp CLI.cpp commands.cpp defaultIO.cpp
HEADERS = distance.h euc.h man.h cheb.h can.h mink.h knn.h CLI.h client.h commands.h defaultIO.h server.h
CC = g++
CXXFLAGS=-g -c -Wall -pthread

all: $(TARGETS)

server.out: $(server_OBJS)
	$(CC) -o $@ $^ -pthread

client.out: $(client_OBJS)
	$(CC) -o $@ $^ -pthread

%.o: %.cpp
	$(CC) $(CXXFLAGS) -o $@ $<

# server.o: server.cpp server.h distance.h euc.h man.h cheb.h can.h mink.h knn.h CLI.h defaultIO.h
# client.o: client.cpp client.h distance.h euc.h man.h cheb.h can.h mink.h knn.h defaultIO.h
# distance.o: distance.cpp distance.h
# knn.o: knn.cpp knn.h distance.h euc.h man.h cheb.h can.h mink.h
# CLI.o: CLI.cpp CLI.h
# commands.o: commands.cpp commands.h
# defaultIO.o: defaultIO.cpp defaultIO.h

clean:
	rm -f *.o $(TARGETS)

