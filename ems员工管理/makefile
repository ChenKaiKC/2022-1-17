all:ser.o cil.o form.o view.o
	gcc cil.o view.o -Wall -lsqlite3 -o cil
	gcc form.o ser.o -Wall -lsqlite3 -pthread -o ser
form.o:form.c inc.h
	gcc -c form.c
cil.o:cil.c inc.h
	gcc -c cil.c -o cil.o

view.o:view.c inc.h
	gcc -c view.c
	
ser.o:ser.c inc.h
	gcc -c ser.c
	
clear:
	rm *.o
