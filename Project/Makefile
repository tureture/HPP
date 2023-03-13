CFLAGS=-Wall -O3 -fopenmp
GCCVERSION=-12
LIBS=
sudoku: sudoku.c
	gcc$(GCCVERSION) $(CFLAGS) -o sudoku sudoku.c $(LIBS)

sudoku_serial: sudoku_serial.c
	gcc$(GCCVERSION) $(CFLAGS) -o sudoku_serial sudoku_serial.c $(LIBS)

clean:
	rm -f ./sudoku *.o