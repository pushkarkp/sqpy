HEADERS = Display.h Move.h Orientation.h PathState.h Piece.h Position.h Presence.h Repeat.h SetOf.h Solutions.h Solve.h SquarePyramid.h Symmetry.h
OBJS = Display.o Move.o Orientation.o PathState.o Piece.o Position.o Presence.o Repeat.o SetOf.o Solutions.o Solve.o SquarePyramid.o Symmetry.o
CC=gcc
CFLAGS=-I.

all: sqpy.exe 
clean:
	rm *~ *.o core *.bak *.tmp
%.o: %.c $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS)

sqpy.exe: SqPy.c $(OBJS)
	$(CC) -o $@ SqPy.c $(OBJS)

