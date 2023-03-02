#!/bin/sh
for T in $(seq 1 1 16)
do
        echo "T = $T"
        time ./galsim 10000 input_data/ellipse_N_10000.gal 100 0.00001 0 $T
done
for T in $(seq 18 2 32)
do
        echo "T = $T"
        time ./galsim 10000 input_data/ellipse_N_10000.gal 100 0.00001 0 $T
done
for T in $(seq  48 12 128)
do
        echo "T = $T"
        time ./galsim 10000 input_data/ellipse_N_10000.gal 100 0.00001 0 $T
done