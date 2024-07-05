CC = g++
CFLAGS = -Wall -O3 --std=c++11

all: continuum.exe discrete.exe

discrete.exe : main.o random.o
	$(CC) random.o main.o -o discrete.exe
main.o : discrete.cpp
	$(CC) -c discrete.cpp -o main.o $(CFLAGS)
continuum.exe : main2.o random.o
	$(CC) random.o main2.o -o continuum.exe
main2.o : continuum.cpp
	$(CC) -c continuum.cpp -o main2.o $(CFLAGS)
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)
 

  
clean :
	rm *.o main.exe seed.out
