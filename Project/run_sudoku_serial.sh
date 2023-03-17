#!/bin/sh

for N in $(seq 0 4 176)
do
    time ./sudoku_serial 4 ordered_input/input_$N.txt out.txt


done