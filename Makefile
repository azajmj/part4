OBJS	= distance.o knn.o main.o server.ot client.o
SOURCE	= distance.cpp knn.cpp main.cpp
HEADER	= distance.h euc.h man.h cheb.h can.h mink.h knn.h
OUT	= a.out
CC	 = g++
FLAGS	 = -g -c -Wall
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

distance.o: distance.cpp
	$(CC) $(FLAGS) distance.cpp -std=c++17

knn.o: knn.cpp
	$(CC) $(FLAGS) knn.cpp -std=c++17
  
server.o: server.cpp
	$(CC) $(FLAGS) distance.cpp -std=c++17

client.o: client.cpp
	$(CC) $(FLAGS) knn.cpp -std=c++17
 

main.o: main.cpp
	$(CC) $(FLAGS) main.cpp -std=c++17


clean:
	rm -f $(
