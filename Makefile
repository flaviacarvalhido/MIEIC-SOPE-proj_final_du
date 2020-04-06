proj: proj.o log.o structparse.o
	gcc proj.o log.o structparse.o -Wall -o proj

proj.o: proj.c
	gcc -c proj.c

log.o: log.c log.h
	gcc -c log.c

structparse.o: structparse.c
	gcc -c structparse.c


clean:
	rm *.o proj
