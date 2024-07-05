CC = g++
CFLAGS = -Wall -O3 --std=c++11

all: Chi2.exe main.exe

main.exe : main.o random.o
	$(CC) random.o main.o -o main.exe
main.o : main.cpp
	$(CC) -c main.cpp -o main.o $(CFLAGS)
Chi2.exe : main2.o random.o
	$(CC) random.o main2.o -o Chi2.exe
main2.o : Chi2.cpp
	$(CC) -c Chi2.cpp -o main2.o $(CFLAGS)
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)
 

  
clean :
	rm *.o seed.out
