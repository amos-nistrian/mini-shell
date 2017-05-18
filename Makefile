#
#comments --here--
#
#
CC = clang
CFLAGS =  -g -Wall -ferror-limit=100 #-fmax-errors=2 for gcc, -ferror-limit=2 for clang
LDFLAGS = -g -lm -L/home/finkec/lib
libs = -lbsd
OBJECTS =  CLI.o calc.o listf.o
EXES = CLI calc listf


CLI:	CLI.o
	$(CC) CLI.c -o CLI $(LDFLAGS) $(libs)

calc:   calc.o
	$(CC) calc.c -o calc $(LDFLAGS)

listf:  listf.o
	$(CC) listf.c -o listf $(LDFLAGS) $(libs)

all:
	$(MAKE) $(EXES)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) $(EXES)
