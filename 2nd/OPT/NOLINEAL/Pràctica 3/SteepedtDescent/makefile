include options.mk

all: SteepestDescend.exe

SteepestDescend.exe: main.o SteepestDescend.o func.o
	gcc main.o SteepestDescend.o func.o -L$(LIBPATH) -lgsl -lgslcblas -lm -o SteepestDescend.exe -static

main.o: main.c
	gcc -c main.c -Wall -I$(INCLUDEPATH) -o main.o
SteepestDescend.o: SteepestDescend.c
	gcc -c SteepestDescend.c -Wall -I$(INCLUDEPATH) -o SteepestDescend.o
func.o: func.c
	gcc -c func.c -Wall -I$(INCLUDEPATH) -o func.o

clean: 
	$(DELETE) *.o
	$(DELETE) *.exe
