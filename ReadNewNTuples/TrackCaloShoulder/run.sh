#!/bin/bash

#for file in `ls /unix/muons/g-2/omegaEU/v9_17_01_new/gm2*root`; do
for file in `ls ../../../Ntuples/60hr_v9_17_01/gm2*root`; do
    echo $file
    ./PlotterTrackCalo $file
done

hadd RootFiles/TrackCalo.root plots*.root
