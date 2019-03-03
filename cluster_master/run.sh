#!/bin/bash

for file in `ls /unix/muons/g-2/omegaEU/60hr_v9_11_00/gm2*root`; do

    echo $file
    # ignore truncated file 
    [[ $file == *30.root ]] && continue ; 

    ./Plotter_Master_Cluster $file

done

hadd merged_cluster.root plots*.root
