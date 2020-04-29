CFLAG = -DDEBUG -Wall -std=c11
# CFLAG = -std=c99
main: main.o process.o schedule.o
	gcc $(CFLAG) main.o process.o schedule.o -o main
main.o: main.c Makefile
	gcc $(CFLAG) main.c -c
schedule.o: schedule.c schedule.h Makefile
	gcc $(CFLAG) schedule.c -c
process.o: process.c process.h Makefile
	gcc $(CFLAG) process.c -c

clean:
	rm -rf *.o

run:
	sudo ./main
