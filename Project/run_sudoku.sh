#!/bin/sh
THREADS=4
for N in $(seq 0 4 176)
do
    time ./sudoku 4 ordered_input/input_$N.txt out.txt $THREADS


done