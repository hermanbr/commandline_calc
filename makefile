


main: main.o
	gcc -o main main.o -lm

main.o: main.c
	gcc -c -g -Wall main.c

clean:
	rm -f main main.o