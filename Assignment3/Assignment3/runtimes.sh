#!/bin/sh
for N in $(seq 10 10 100)
do
        echo "---------------"
        start=`date +%s`
        ./galsim $N input_data/ellipse_N_10000.gal 1000 0.00001 0
        end=`date +%s`
        runtime=$((end-start))
        echo "N: $N with taken time: $(echo "scale=3;$runtime/1000" | bc) seconds"
        echo "---------------"
done
for N in $(seq 200 100 1000)
do
        echo "---------------"
        start=`date +%s`
        ./galsim $N input_data/ellipse_N_10000.gal 1000 0.00001 0
        end=`date +%s`
        runtime=$((end-start))
        echo "N: $N with taken time: $(echo "scale=3;$runtime/1000" | bc) seconds"
        echo "---------------"
done
for N in $(seq 2000 2000 10000)
do
        echo "---------------"
        start=`date +%s`
        ./galsim $N input_data/ellipse_N_10000.gal 1000 0.00001 0
        end=`date +%s%3`
        runtime=$((end-start))
        echo "N: $N with taken time: $(echo "scale=3;$runtime/1000" | bc) seconds"
        echo "---------------"
done