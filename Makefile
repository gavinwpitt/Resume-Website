CC=gcc
CFLAGS= -lncurses -l -L

test: jerryXtreme.c
	gcc -std=c99 -Wall -Wextra -pedantic -o jerryXtreme jerryXtreme.c -lncurses
