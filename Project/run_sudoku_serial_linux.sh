#!/bin/sh
for N in $(seq 0 4 178)
do
        ./sudoku_serial 4 ordered_input/input_$N.txt out.txt
        echo "Size $N"
done