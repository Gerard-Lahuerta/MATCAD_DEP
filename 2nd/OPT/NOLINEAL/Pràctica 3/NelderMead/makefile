include options.mk

all: NelderMead.exe

NelderMead.exe: main.o NelderMead.o func.o
	gcc main.o NelderMead.o func.o -L$(LIBPATH) -lgsl -lgslcblas -lm -o NelderMead.exe -static

main.o: main.c
	gcc -c main.c -Wall -I$(INCLUDEPATH) -o main.o
NelderMead.o: NelderMead.c
	gcc -c NelderMead.c -Wall -I$(INCLUDEPATH) -o NelderMead.o
func.o: func.c
	gcc -c func.c -Wall -I$(INCLUDEPATH) -o func.o

clean: 
	$(DELETE) *.o
	$(DELETE) *.exe