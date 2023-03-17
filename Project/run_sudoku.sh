#!/bin/sh
THREADS=4
for N in $(seq 0 4 176)
do
    echo "Size $N"
    echo "Sudoku parallel random"
    time ./sudoku 4 ordered_input/input_$N.txt $THREADS

    echo "Sudoku serial"
    time ./sudoku_serial 4 ordered_input/input_$N.txt out

    echo "Sudoku parallel openMP"
    time ./sudoku_serial 4 ordered_input/input_$N.txt $THREADS

done