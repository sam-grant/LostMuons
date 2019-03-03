// General plotter using the tracker matched tree
// Alter Plotter.h accordingly

// gavin.hesketh@ucl.ac.uk
// samuel.grant.18@ucl.ac.uk

#define Plotter_cxx
#include "Plotter.h"
#include "TMath.h"

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

  plot2DTr("pt",500,0,126000,500,0,3500, "Time [ns]", "Momentum [MeV]");

  plot1DTr("y_track", 500, -120, 120, "Decay Vertex Y [mm]", "Number of Hits");

  plot1DTr("radius", 500, -200, 200, "Radius [mm]", "Number of Hits");

  plot2DTr("xy_calo", 500,-150, 150, 500,-120, 120, "Calo Decay Vertex X [mm]", "Calo Decay Vertex Y [mm]");

  plot1DTr("mom", 500,0, 3500,"Momentum [MeV]","Number of Hits");

  plot1DTr("time", 500, 0, 126000,"Time [ns]","Number of Hits");

  plot2DTr("xy_calo_fit", 500, -150, 150, 500, -120, 120,"Calo Decay Vertex X [mm]","Calo Decay Vertex Y [mm]");  

}


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
 
      Fill2DTr("pt",t,p);
      Fill1DTr("y_track", TrY);
      Fill1DTr("radius",R);
      Fill2DTr("xy_calo",cTrX,cTrY);
      Fill1DTr("mom",p);
      Fill1DTr("time",t);
      Fill2DTr("xy_calo_fit",cTrX,cTrY);
	
 
  } //event loop



  delete tr;
  return;

}





