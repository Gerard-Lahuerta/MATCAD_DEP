include options.mk

all: PR.exe

PR.exe: main.o PR.o func.o
	gcc main.o PR.o func.o -L$(LIBPATH) -lgsl -lgslcblas -lm -o PR.exe -static

main.o: main.c
	gcc -c main.c -Wall -I$(INCLUDEPATH) -o main.o
PR.o: PR.c
	gcc -c PR.c -Wall -I$(INCLUDEPATH) -o PR.o
func.o: func.c
	gcc -c func.c -Wall -I$(INCLUDEPATH) -o func.o

clean: 
	$(DELETE) *.o
	$(DELETE) *.exe
