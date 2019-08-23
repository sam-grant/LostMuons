#!/bin/bash

for file in `ls ../../../Ntuples/60hr_v9_17_01_Mini/gm2*.root`; do
    echo $file
    ./PlotterTrack $file
done

hadd RootFiles/TrackCandidate.root plots*.root
