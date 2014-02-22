CC = gcc
CFILES = main.c viewport.c map.c
INCLUDES = 
CFLAGS =
SLIBS = -lncurses
PROGRAM = solace

OBJECTS = $(CFILES:.c=.o)

debug := CFLAGS= -g

all debug: $(PROGRAM)

$(PROGRAM): $(INCLUDES) $(OBJECTS)
	$(LINK.c) -o $@ $(OBJECTS) $(SLIBS)

clean:
	rm -f $(PROGRAM) $(OBJECTS)
