proj: proj.o log.o structparse.o aux.o
	gcc proj.o log.o structparse.o aux.o -Wall -o proj

proj.o: proj.c
	gcc -c proj.c

log.o: log.c log.h
	gcc -c log.c

structparse.o: structparse.c
	gcc -c structparse.c

aux.o: aux.c aux.h
		gcc -c aux.c

clean:
	rm *.o proj
