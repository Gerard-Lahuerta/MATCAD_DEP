include options.mk

all: checkdf.exe

func.o: func.c
	gcc -c func.c -Wall -I$(INCLUDEPATH) -o func.o

checkdf.exe: checkdf.o func.o 
	gcc checkdf.o func.o -L$(LIBPATH) -lgsl -lgslcblas -lm -o checkdf.exe -static

checkdf.o: 
	gcc -c checkdf.c -Wall -I$(INCLUDEPATH) -o checkdf.o

clean: 
	$(DELETE) *.o
	$(DELETE) *.exe