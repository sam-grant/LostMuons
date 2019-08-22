Sam Grant 2019 

Data used: 60hr_v9_11_00
Resolution : At 1.5 GeV, the resolutions are 1 mm on the radial position, 1.5 mm on the vertical position, 0.5% on the curvature, and 1 mrad on the vertical angle. 

How does lost muon contamination alter up beam position measurements. The final four numbers will be the shift in the radial/vertical means/wdiths. Use tracker only tree as reference, remove lost muons in the cluster-tracker tree. Scale up cluster-tracker results according to relative efficiency of the calo, and compare. 

Track level cuts:
p < 0.05
hitVolume = True
Calo vertex level cuts:
-10 ns < t_cluster - t_track < 6 ns
dR < 30 mm

ReadNTuples / Plotter Code:

PlotterTrack.C : Reads the tracker only branch, where one event is one track
PlotterTrackCluster.C : Reads the track-cluster branch, where one event is one track-cluster match
PlotterTrackClusterHighEnergyCut.C : Reads the track-cluster branch, applies additional high energy cut in the lost muon momentum regime. This removes the high momentum shoulder in the non lost muon momentum distribution.
PlotterTrackClusterLowRadius.C : Reads the track-cluster branch. Looks at slice of tracks at low radius, visible as a bump in the radial distribution. This is shown to be MIPS, fake MIPs, lost muons, and pileup.
PlotterTrackClusterFakeMIPs.C : Reads the track-cluster branch. Looks at the higher energy region above the lost muon region in E vs p, and looks at what this is. (It's fake MIPS I think). 


   





