CC = c++
CFLAGS = -std=c++20 -Wall

all: lab3

lab3: main.o
	$(CC) $(CFLAGS) -o lab3 main.o

main.o: main.cpp 
	$(CC) $(CFLAGS) -c main.cpp

clean:
	rm -f main.o lab3