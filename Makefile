HEADERS = Display.h Move.h Orientation.h Path.h PathState.h Piece.h Position.h Presence.h Repeat.h SetOf.h Solutions.h Solve.h SquarePyramid.h Steps.h Symmetry.h Topics.h
CC=gcc
CFLAGS=-I. -O2 -g
OBJDIR=obj
objs = $(addprefix $(OBJDIR)/, Display.o Move.o Orientation.o Path.o PathState.o Piece.o Position.o Presence.o Repeat.o SetOf.o Solutions.o Solve.o SquarePyramid.o Steps.o Symmetry.o Topics.o )
GPROF_FLAGS=-I. -O2 -pg
GPROF_OBJDIR=gprofobj
gprof_objs = $(addprefix $(GPROF_OBJDIR)/, Display.o Move.o Orientation.o Path.o PathState.o Piece.o Position.o Presence.o Repeat.o SetOf.o Solutions.o Solve.o SquarePyramid.o Steps.o Symmetry.o Topics.o )

sqpy.exe: SqPy.c $(objs)
	$(CC) $(CFLAGS) -o $@ SqPy.c $(objs)

$(OBJDIR)/%.o: %.c $(HEADERS) | $(OBJDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIR) : 
	mkdir $(OBJDIR)

gprof: sqpyGprof.exe

sqpyGprof.exe: SqPy.c $(gprof_objs)
	$(CC) $(GPROF_FLAGS) -o $@ SqPy.c $(gprof_objs)

$(GPROF_OBJDIR)/%.o: %.c $(HEADERS) | $(GPROF_OBJDIR)
	$(CC) $(GPROF_FLAGS) -c -o $@ $<

$(GPROF_OBJDIR) : 
	mkdir $(GPROF_OBJDIR)

clean:
	-rm -Rf $(OBJDIR) $(GPROF_OBJDIR) *~ *.o core *.bak *.tmp *.stackdump *.coredump
