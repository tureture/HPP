CFLAGS=-Wall -O3 -ffast-math -lpthread
INCLUDES=-I/opt/X11/include
LDFLAGS=-L/opt/X11/lib -lX11 -lm
GCCVERSION=-12

galsim: galsim.o graphics.o
	gcc$(GCCVERSION) -o galsim galsim.o graphics.o $(LDFLAGS)

galsim.o: galsim.c graphics/graphics.h
	gcc$(GCCVERSION) $(CFLAGS) $(INCLUDES) -c galsim.c

graphics.o: graphics/graphics.c graphics/graphics.h
	gcc$(GCCVERSION) $(CFLAGS) $(INCLUDES) -c graphics/graphics.c

clean:
	rm -f ./galsim *.o

galsim_noG: galsim_noG.o graphics.o
	gcc$(GCCVERSION) -o galsim_noG galsim_noG.o $(LDFLAGS)

galsim_noG.o: galsim_noG.c
	gcc$(GCCVERSION) $(CFLAGS) $(INCLUDES) -c galsim_noG.c


