#!/bin/sh
echo "N [num]-time pthreads [s]-time 1_pthreads [s]-time 2_pthreads [s]-time openmp [s]-time 1_openmp [s]-time 2_openmp [s]" > ./runtimes_compare_N.txt
for N in $(seq 1 1 10)
do
        echo "---------------"
        echo "N: $N"
        start=`date +%s%3N`
        ./galsim_pthreads $N input_data/ellipse_N_10000.gal 100 0.00001 0 16
        end=`date +%s%3N`
        runtime_pthreads=$((end-start))
        echo "pthreads: $(echo "scale=3;$runtime_pthreads/1000" | bc) seconds"
        start=`date +%s%3N`
        ./galsim_openmp $N input_data/ellipse_N_10000.gal 100 0.00001 0 16
        end=`date +%s%3N`
        runtime_openmp=$((end-start))
        echo "openmp: $(echo "scale=3;$runtime_openmp/1000" | bc) seconds"
        start=`date +%s%3N`
        ./1_galsim_pthreads $N input_data/ellipse_N_10000.gal 100 0.00001 0 16
        end=`date +%s%3N`
        runtime_pthreads_1=$((end-start))
        echo "1_pthreads: $(echo "scale=3;$runtime_pthreads_1/1000" | bc) seconds"
        start=`date +%s%3N`
        ./1_galsim_openmp $N input_data/ellipse_N_10000.gal 100 0.00001 0 16
        end=`date +%s%3N`
        runtime_openmp_1=$((end-start))
        echo "1_openmp: $(echo "scale=3;$runtime_openmp_1/1000" | bc) seconds"
        start=`date +%s%3N`
        ./2_galsim_pthreads $N input_data/ellipse_N_10000.gal 100 0.00001 0 16
        end=`date +%s%3N`
        runtime_pthreads_2=$((end-start))
        echo "2_pthreads: $(echo "scale=3;$runtime_pthreads_2/1000" | bc) seconds"
        start=`date +%s%3N`
        ./2_galsim_openmp $N input_data/ellipse_N_10000.gal 100 0.00001 0 16
        end=`date +%s%3N`
        runtime_openmp_2=$((end-start))
        echo "2_openmp: $(echo "scale=3;$runtime_openmp_2/1000" | bc) seconds"
        echo "---------------"
        echo "$N-$(echo "scale=3;$runtime_pthreads/1000" | bc)-$(echo "scale=3;$runtime_pthreads_1/1000" | bc)-$(echo "scale=3;$runtime_pthreads_2/1000" | bc)-$(echo "scale=3;$runtime_openmp/1000" | bc)-$(echo "scale=3;$runtime_openmp_1/1000" | bc)-$(echo "scale=3;$runtime_openmp_2/1000" | bc)" >> ./runtimes_compare_N.txt
done
for n in $(seq 1 1 3)
do
	for N in $(seq $(echo "10^$n+10^$(($n-1))" | bc) $(echo "10^$(($n-1))" | bc) $(echo "10^$(($n+1))" | bc))
	do
	        echo "---------------"
        	echo "N: $N"
        	start=`date +%s%3N`
        	./galsim_pthreads $N input_data/ellipse_N_10000.gal 100 0.00001 0 16
        	end=`date +%s%3N`
        	runtime_pthreads=$((end-start))
        	echo "pthreads: $(echo "scale=3;$runtime_pthreads/1000" | bc) seconds"
        	start=`date +%s%3N`
        	./galsim_openmp $N input_data/ellipse_N_10000.gal 100 0.00001 0 16
        	end=`date +%s%3N`
        	runtime_openmp=$((end-start))
        	echo "openmp: $(echo "scale=3;$runtime_openmp/1000" | bc) seconds"
        	start=`date +%s%3N`
        	./1_galsim_pthreads $N input_data/ellipse_N_10000.gal 100 0.00001 0 16
        	end=`date +%s%3N`
        	runtime_pthreads_1=$((end-start))
        	echo "1_pthreads: $(echo "scale=3;$runtime_pthreads_1/1000" | bc) seconds"
        	start=`date +%s%3N`
        	./1_galsim_openmp $N input_data/ellipse_N_10000.gal 100 0.00001 0 16
        	end=`date +%s%3N`
        	runtime_openmp_1=$((end-start))
        	echo "1_openmp: $(echo "scale=3;$runtime_openmp_1/1000" | bc) seconds"
        	start=`date +%s%3N`
        	./2_galsim_pthreads $N input_data/ellipse_N_10000.gal 100 0.00001 0 16
        	end=`date +%s%3N`
        	runtime_pthreads_2=$((end-start))
        	echo "2_pthreads: $(echo "scale=3;$runtime_pthreads_2/1000" | bc) seconds"
        	start=`date +%s%3N`
        	./2_galsim_openmp $N input_data/ellipse_N_10000.gal 100 0.00001 0 16
       		end=`date +%s%3N`
        	runtime_openmp_2=$((end-start))
        	echo "2_openmp: $(echo "scale=3;$runtime_openmp_2/1000" | bc) seconds"
        	echo "---------------"
        	echo "$N-$(echo "scale=3;$runtime_pthreads/1000" | bc)-$(echo "scale=3;$runtime_pthreads_1/1000" | bc)-$(echo "scale=3;$runtime_pthreads_2/1000" | bc)-$(echo "scale=3;$runtime_openmp/1000" | bc)-$(echo "scale=3;$runtime_openmp_1/1000" | bc)-$(echo "scale=3;$runtime_openmp_2/1000" | bc)" >> ./runtimes_compare_N.txt
	done
done