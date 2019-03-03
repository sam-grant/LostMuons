//Simple code to make plots from the Europa OmegaA ntuples
// gavin.hesketh@ucl.ac.uk
// samuel.grant.18@ucl.ac.uk

/* HAD TROUBLE WITH SOME BUG PREVENTING THE MAKEFILE FROM COMPILING IF PLOTTER_LOST_MUONS IS RENAMED. THIS CODE IS FOR CLUSTER TRACKER TRACKER ONLY */


//to use a particular branch, make sure it is uncommented in Reader.C
//branch variables are listed in Reader.h

#define Plotter_cxx
#include "Plotter.h"
#include "TMath.h"

//using namespace std;

const double pi = 3.14159265359;

int CaloNum(int caloX, int caloY) {
  return caloX+9*caloY;
}
void Plotter::InitTrees(TString input_file) {
  //initialise the trees you want to read
  //then enable the relevant branches in Reader.C
  //  ctt = new clusterTrackerTrackReader(input_file, "clusterTracker");
  // cr = new clusterTrackerClusterReader(input_file);
  // cl = new clusterReader(input_file);
  tr = new trackerReader(input_file);
}



void Plotter::InitHistos() {

  for (int i = 0; i < 2; i++) {
    
    // slice 0: fiducial | slice 1: non-fiducial 

    plot2DTr("pt_"+std::to_string(i),500,0,126000,500,0,3500, "Time [ns]", "Momentum [MeV]");

    plot1DTr("y_track_"+std::to_string(i), 500, -120, 120, "Decay Vertex Y [mm]", "Number of Hits");

    plot1DTr("radius_"+std::to_string(i), 500, -200, 200, "Radius [mm]", "Number of Hits");

    plot2DTr("xy_calo_"+std::to_string(i), 500,-150, 150, 500,-120, 120, "Calo Decay Vertex X [mm]", "Calo Decay Vertex Y [mm]");

    plot1DTr("mom_"+std::to_string(i), 500,0, 3500,"Momentum [MeV]","Number of Hits");

    plot1DTr("time_"+std::to_string(i), 500, 0, 126000,"Time [ns]","Number of Hits");

    plot2DTr("xy_calo_fit_"+std::to_string(i), 500, -150, 150, 500, -120, 120,"Calo Decay Vertex X [mm]","Calo Decay Vertex Y [mm]");  


  }



}
//=========================================================

//loop over the entries in the tree, making plots:

void Plotter::Run() {
  
  //loop over the clusterTracker/tracker tree:
  while( NextTrEvent() ) {
     
    // Volume cut, P value cut, Momentum cut at 2.3 GeV====

    if(tr->hitVolume == true) continue;
    if(tr->trackPValue < 0.05) continue;

    //classify the tracks:
    bool lostMu=false;

    // Track Momentum
    const double p = tr -> trackMomentum;
  
    //====Calo Decay Vertices====
    double cTrX = tr->caloVertexPosX;
    double cTrY = tr->caloVertexPosY;

    //====Tracker decay vertices====
    double TrX = tr->decayVertexPosX; 
    double TrY = tr->decayVertexPosY;
    double TrZ = tr->decayVertexPosZ;

    //====Tracker decay times, ns====
    double t = (tr -> decayTime);
    //====cut 30 us, completely wipes out lost muons====
    // if (t < 30000) continue;    

    //====Decay radii, tracker and calo====
    const double magicRadius = 7112; //[mm]

    const double R = sqrt(TrX*TrX+TrZ*TrZ) - magicRadius;

    //====Angle of rotation in xz plane, dependant on quadrant====
    /* xz plane with theta going clockwise, 12 o'clock: inflector, station 1
       6 o'clock we have station 13
       9 o'clock we have station 19 */
    double theta = 0;

    if ( TrX > 0 && TrZ > 0) { // quad 1
      theta = atan(TrZ/TrX);
    } 
    else if ( TrX < 0 && TrZ > 0 ) { // quad 2
      theta = -1 * atan(TrX/TrZ) + pi/2 ;
    } 
    else if (  TrX < 0 && TrZ < 0 ) { // quad 3
      theta = atan(TrZ/TrX) + pi;
    } 
    else if ( TrX > 0 && TrZ < 0 ) { // quad 4
      theta = -1 * atan(TrX/TrZ)+ 3*pi/2; 
    }
   
    // Clean up zeros
    if (TrX == 0 && TrY == 0 && TrZ == 0) continue;


    // Hard cut on radius to get rid of low tail
    if (R > 200 || R < -200) continue;
    // Fidciual cut 
    int k;

    if (Fiducial(cTrX, cTrY) == true) {k=0;}
    else if (Fiducial(cTrX, cTrY) == false) {k=1;}
   
    Fill2DTr("pt_"+std::to_string(k),t,p);
    Fill1DTr("y_track_"+std::to_string(k), TrY);
    Fill1DTr("radius_"+std::to_string(k),R);
    Fill2DTr("xy_calo_"+std::to_string(k),cTrX,cTrY);
    Fill1DTr("mom_"+std::to_string(k),p);
    Fill1DTr("time_"+std::to_string(k),t);
    Fill2DTr("xy_calo_fit_"+std::to_string(k),cTrX,cTrY);
       
  } //event loop



  delete tr;
  return;

}





