include options.mk

all: brent.exe

brent.exe: main.o brent.o func.o
	gcc main.o brent.o func.o -L$(LIBPATH) -lm -lgsl -lgslcblas -o brent.exe -static

main.o: main.c
	gcc -c main.c -Wall -I$(INCLUDEPATH) -o main.o
brent.o: brent.c
	gcc -c brent.c -Wall -I$(INCLUDEPATH) -o brent.o
func.o: func.c
	gcc -c func.c -Wall -I$(INCLUDEPATH) -o func.o

clean: 
	$(DELETE) *.o
	$(DELETE) *.exe
