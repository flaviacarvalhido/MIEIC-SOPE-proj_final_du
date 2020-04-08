simpledu: simpledu.o log.o structparse.o aux.o
	gcc simpledu.o log.o structparse.o aux.o -Wall -o simpledu

simpledu.o: simpledu.c
	gcc -c simpledu.c

log.o: log.c log.h
	gcc -c log.c

structparse.o: structparse.c
	gcc -c structparse.c

aux.o: aux.c aux.h
		gcc -c aux.c

clean:
	rm *.o simpledu
