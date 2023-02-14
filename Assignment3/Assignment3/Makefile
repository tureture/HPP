CFLAGS=-Wall
INCLUDES=-I/opt/X11/include
LDFLAGS=-L/opt/X11/lib -lX11 -lm

galsim: galsim.o graphics.o
	gcc -o galsim galsim.o graphics.o $(LDFLAGS)

galsim.o: galsim.c graphics/graphics.h
	gcc $(CFLAGS) $(INCLUDES) -c galsim.c

graphics.o: graphics/graphics.c graphics/graphics.h
	gcc $(CFLAGS) $(INCLUDES) -c graphics/graphics.c

clean:
	rm -f ./galsim *.o
