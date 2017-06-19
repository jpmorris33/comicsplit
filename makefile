#
#	Makefile for comicsplit
#


all: comicsplit

CC                = g++
CFLAGS            = -I. -g -O6 -Wall
LD		  = g++
LDFLAGS		  =  `libpng-config --libs` -lstdc++

Object_files   = main.o image.o process.o readpng.o writepng.o


comicsplit: $(Object_files)
	-rm -f $(comicsplit)
	$(LD)  $^ $(comicsplit) $(LDFLAGS) -o comicsplit

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

%.o: %.cc
	$(CC) -c $(CFLAGS) $< -o $@

%.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@

%.o: %.asm
	$(ASM) -f coff $< -o $@

clean:
	rm -f *.o $(comicsplit)
