This folder contistutes the analysis for the 05/03/2016 g-2 tracker meeting. 

Data used: /unix/muons/g-2/omegaEU/v9_11_00

Ultimately, the aim of the exercise is to investigate how lost muon contamination messes up beam position measurements. The final four numbers will be the difference in the radial/vertical means and width of the all tracker tree with and without scaled lost muons subtracted. 

Cuts all:
p < 0.05
hitVolume = True

Cuts Cluster:
-10 ns < t_cluster - t_track < 6 ns
dR < 30 mm

MakePlots code
--------------
FractionFitter.C : Performs a fractional fit to determine the relative efficiency of the calo for lost muons and non lost muons at high p.

xyCalo.C : Generates all the xy decay vertex calorimeter plots

MomentumAndEnergy.C : Generates energy and momentum distributions from cluster_master

EvsP.C : Generates E vs. p plots from cluster_master

Radii.C : Generates radii plots for tracker, track minus lost muons, and tracker minus non lost muons with and without scaling. 

Vertical.C : Generates vertical plots for tracker, track minus lost muons, and tracker minus non lost muons with and without scaling. 
  





