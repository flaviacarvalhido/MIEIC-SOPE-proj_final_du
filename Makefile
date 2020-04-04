proj: proj.o log.o
	gcc proj.o log.o -Wall -o proj

proj.o: proj.c
	gcc -c proj.c

log.o: log.c log.h
	gcc -c log.c


clean:
	rm *.o proj
