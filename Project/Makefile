CFLAGS=-Wall -O3 -fopenmp 
GCCVERSION=-12
LIBS=

sudoku: sudoku.c
	gcc$(GCCVERSION) $(CFLAGS) -o sudoku sudoku.c $(LIBS)

sudoku_serial: sudoku_serial.c
	gcc$(GCCVERSION) $(CFLAGS) -o sudoku_serial sudoku_serial.c $(LIBS)

sudoku_memcopy: sudoku_memcopy.c
	gcc$(GCCVERSION) $(CFLAGS) -o sudoku_memcopy sudoku_memcopy.c $(LIBS)

sudoku_openmp_test: sudoku_openmp_test.c
	gcc$(GCCVERSION) $(CFLAGS) -o sudoku_openmp_test sudoku_openmp_test.c $(LIBS)

sudoku_pthread: sudoku_pthreads.c
	gcc$(GCCVERSION) $(CFLAGS) -o sudoku_pthread sudoku_pthreads.c $(LIBS)

sudoku_test_stackoverflow: sudoku_test_stackoverflow.c
	gcc$(GCCVERSION) $(CFLAGS) -o sudoku_test_stackoverflow sudoku_test_stackoverflow.c $(LIBS)

clean:
	rm -f ./sudoku *.o