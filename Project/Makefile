CFLAGS=-Wall -O3 -Xclang -fopenmp
GCCVERSION=
LIBS=-lomp
sudoku: sudoku.c
	gcc$(GCCVERSION) -o sudoku sudoku.c $(LIBS)

clean:
	rm -f ./sudoku *.o