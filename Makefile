HEADERS = Display.h PathState.h Piece.h Position.h Presence.h Repeat.h Solutions.h Solve.h SquarePyramid.h
OBJS = Display.o PathState.o Piece.o Position.o Presence.o Repeat.o Solutions.o Solve.o SquarePyramid.o
CC=gcc
CFLAGS=-I.

all: sqpy.exe 
clean:
	rm *~ *.o core *.bak *.tmp
%.o: %.c $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS)

sqpy.exe: SqPy.c $(OBJS)
	$(CC) -o $@ SqPy.c $(OBJS)

