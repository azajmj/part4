OBJS	= distance.o knn.o CLI.o commands.o defaultIO.o main.o
SOURCE	= distance.cpp knn.cpp CLI.cpp commands.cpp defaultIO.cpp main.cpp
HEADER	= distance.h euc.h man.h cheb.h can.h mink.h knn.h CLI.h commands.h defaultIO.h
OUT	= out
CC	 = g++
FLAGS	 = -g -c -Wall
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

distance.o: distance.cpp
	$(CC) $(FLAGS) distance.cpp -std=c++17

knn.o: knn.cpp
	$(CC) $(FLAGS) knn.cpp -std=c++17

CLI.o: CLI.cpp
	$(CC) $(FLAGS) CLI.cpp -std=c++17

commands.o: commands.cpp
	$(CC) $(FLAGS) commands.cpp -std=c++17

defaultIO.o: defaultIO.cpp
	$(CC) $(FLAGS) defaultIO.cpp -std=c++17

main.o: main.cpp
	$(CC) $(FLAGS) main.cpp -std=c++17


clean:
	rm -f $(OBJS) $(OUT)