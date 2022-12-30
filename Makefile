TARGETS = server.out client.out
server_OBJS = distance.o knn.o server.o
client_OBJS = distance.o knn.o client.o
SOURCES = distance.cpp knn.cpp server.cpp client.cpp
HEADERS = distance.h euc.h man.h cheb.h can.h mink.h knn.h
CC = g++
FLAGS = -g -c -Wall

all: $(TARGETS)

server.out: $(server_OBJS)
	$(CC) -o $@ $^

client.out: $(client_OBJS)
	$(CC) -o $@ $^

%.o: %.cpp $(HEADERS)
	$(CC) $(FLAGS) -o $@ $<

clean:
	rm -f *.o $(TARGETS)
