include options.mk

all: goldenSection.exe

goldenSection.exe: main.o goldenSection.o func.o
	gcc main.o goldenSection.o func.o -L$(LIBPATH) -lm -lgsl -lgslcblas -o goldenSection.exe -static

main.o: main.c
	gcc -c main.c -Wall -I$(INCLUDEPATH) -o main.o
goldenSection.o: goldenSection.c
	gcc -c goldenSection.c -Wall -I$(INCLUDEPATH) -o goldenSection.o
func.o: func.c
	gcc -c func.c -Wall -I$(INCLUDEPATH) -o func.o

clean: 
	$(DELETE) *.o
	$(DELETE) *.exe