#!/bin/bash

for file in `ls ../../../../60hr_v9_17_01/gm2*root`; do

    echo $file
    # ignore truncated file 
#    [[ $file == *30.root ]] && continue ; 

    ./plotter_slices $file

done

hadd merged_tracker_slices.root plots*.root
