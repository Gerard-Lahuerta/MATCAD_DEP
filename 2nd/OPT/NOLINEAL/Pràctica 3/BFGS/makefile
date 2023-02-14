include options.mk

all: BFGS.exe

BFGS.exe: main.o BFGS.o func.o
	gcc main.o BFGS.o func.o -L$(LIBPATH) -lgsl -lgslcblas -lm -o BFGS.exe -static

main.o: main.c
	gcc -c main.c -Wall -I$(INCLUDEPATH) -o main.o
BFGS.o: BFGS.c
	gcc -c BFGS.c -Wall -I$(INCLUDEPATH) -o BFGS.o
func.o: func.c
	gcc -c func.c -Wall -I$(INCLUDEPATH) -o func.o

clean: 
	$(DELETE) *.o
	$(DELETE) *.exe
