include options.mk

all: FR.exe

FR.exe: main.o FR.o func.o
	gcc main.o FR.o func.o -L$(LIBPATH) -lgsl -lgslcblas -lm -o FR.exe -static

main.o: main.c
	gcc -c main.c -Wall -I$(INCLUDEPATH) -o main.o
FR.o: FR.c
	gcc -c FR.c -Wall -I$(INCLUDEPATH) -o FR.o
func.o: func.c
	gcc -c func.c -Wall -I$(INCLUDEPATH) -o func.o

clean: 
	$(DELETE) *.o
	$(DELETE) *.exe
