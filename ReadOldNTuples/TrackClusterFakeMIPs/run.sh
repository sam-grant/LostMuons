#!/bin/bash

for file in `ls /mnt/strawRaid01/omegaEU/60hr_v9_11_00/gm2rootTrees_ana_*.root`; do

    echo $file

    ./PlotterTrackClusterFakeMIPs $file

done

hadd RootFiles/TrackClusterFakeMIPs.root plots*.root
