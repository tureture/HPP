CFLAGS=-Wall -O3 -ffast-math 
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

galsim_noG: galsim_noG.o graphics.o
	gcc -o galsim_noG galsim_noG.o $(LDFLAGS)

galsim_noG.o: galsim_noG.c
	gcc $(CFLAGS) $(INCLUDES) -c galsim_noG.c


