#!/bin/sh
NUM_THREADS=16
for N in $(seq 0 4 178)
do
        ./sudoku 4 ordered_input/5_$N.txt out.txt $NUM_THREADS
done