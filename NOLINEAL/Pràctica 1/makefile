include options.mk 

all: newton.exe

newton.exe: main.o newton.o func.o
	gcc main.o newton.o func.o -L$(LIBPATH) -lm -lgsl -lgslcblas -o newton.exe -static 

main.o: main.c
	gcc -c main.c -Wall -I$(INCLUDEPATH) -o main.o
newton.o: newton.c
	gcc -c newton.c -Wall -I$(INCLUDEPATH) -o newton.o
func.o: func.c
	gcc -c func.c -Wall -I$(INCLUDEPATH) -o func.o

clean: 
	$(DELETE) *.o
	$(DELETE) *.exe