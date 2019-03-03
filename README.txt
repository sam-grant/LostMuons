This folder contistutes the analysis for the 05/03/2016 g-2 tracker meeting. 

Data used: /unix/muons/g-2/omegaEU/v9_11_00

Ultimately, the aim of the exercise is to investigate how lost muon contamination messes up beam position measurements. The final four numbers will be the difference in the radial/vertical means and width of the all tracker tree with and without scaled lost muons subtracted. 

Cuts all:
p < 0.05
hitVolume = True

Cuts Cluster:
-10 ns < t_cluster - t_track < 6 ns
dR < 30 mm

Plotter Code - In seperate directories so I can generate files in parallel
--------------------------------------------------------------------------
cluster_master : General plotter using the clusterTracker tree

tracker : General plotter using the tracker only tree

cluster_bigCut : Plotter cutting out a larger region of the non lost muon distribution to investigate the possibilty of the original cut being too stringent 

cluster_split : Plotter looking at muon and muon pileup region, comparing the difference in population per run number

cluster_tcut : Plotter for tcut +/- 30 ns with big cut

cluster_shoulder : Just take energy cut of < 2000 MeV && p > 2300 MeV

tracker_fiducial : Apply fiducial volume yes/no selection to the tracker plots 

cluster_fiducial : Apply fiducial volume yes/no selection to the cluster plots

*Note cluster_bigCut and cluster_shoulder should really be incorperated as one


makePlots code
--------------

fractionFitter.C : Performs a fractional fit to determine the relative efficiency of the calo for lost muons and non lost muons at high p.

fractionFiducial.C : Same thing but for fiducial and non fiducal calo volumes

xyCalo.C : Generates all the xy decay vertex calorimeter plots

E_and_p.C : Generates energy and momentum distributions from cluster_master

run_1D_ratio.C : Generates population ratio per run number plots between regions -3.2 < Log(E/p) < -2.4 and -2.4 < Log(E/p) < -1.6

EoP.C : Generates E vs. p plots from cluster_master

EoP_shoulder.C : Generate E vs p plots from cluster_shoulder

EoP_bigCut.C : Generate E vs p plots from cluster_bigCut

EoP_split.C : Generares E vs. p plots for muon regions -3.2 < Log(E/p) < -2.4 and -2.4 < Log(E/p) < -1.6

radii.C : Generates radii plots for tracker, track minus lost muons, and tracker minus non lost muons with and without scaling. 

vertical.C : Generates vertical plots for tracker, track minus lost muons, and tracker minus non lost muons with and without scaling. 
  





