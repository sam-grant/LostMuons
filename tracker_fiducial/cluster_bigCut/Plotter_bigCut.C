//Simple code to make plots from the Europa OmegaA ntuples
// gavin.hesketh@ucl.ac.uk
// samuel.grant.18@ucl.ac.uk

/* This one is for a large block in the E over p plot for the definition of lost muons, see below */

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
   ctt = new clusterTrackerTrackReader(input_file, "clusterTracker");
  // cr = new clusterTrackerClusterReader(input_file);
  // cl = new clusterReader(input_file);
  // tr = new trackerReader(input_file);
}


void Plotter::InitHistos() {
   
  // Lost Muons

  plot2DTr("mu_EoP",500,0,5000,500,0,4000,"Track Momentum [MeV]","Energy in Calo [MeV]");

  plot2DTr("mu_pt",500,0,126000,500,0,3500, "Time [ns]", "Momentum [MeV]");

  plot1DTr("mu_y_track", 500, -120, 120, "Decay Vertex Y [mm]", "Number of Hits");

  plot1DTr("mu_radius", 500, -200, 200, "Radius [mm]", "Number of Hits");

  plot2DTr("mu_xy_calo", 500,-150, 150, 500,-120, 120, "Calo Decay Vertex X [mm]", "Calo Decay Vertex Y [mm]");

  plot1DTr("mu_mom", 500,0, 3500,"Momentum [MeV]","Number of Hits");

  plot1DTr("mu_time", 500, 0, 126000,"Time [ns]","Number of Hits");

  plot1DTr("mu_run", 71, 15921, 15992, "Run Number", "Number of Hits");

  plot1DTr("mu_energy", 500, 0, 4000, "Energy in Calo [MeV]", "Number of Hits");

  plot2DTr("mu_xy_calo_fit", 500, -150, 150, 500, -120, 120,"Calo Decay Vertex X [mm]","Calo Decay Vertex Y [mm]");  

  //  Non Lost Muons

  plot2DTr("n_EoP",500,0,5000,500,0,4000,"Track Momentum [MeV]","Energy in Calo [MeV]");

  plot2DTr("n_pt",500,0,126000,500,0,3500, "Time [ns]", "Momentum [MeV]");

  plot1DTr("n_y_track", 500, -120, 120, "Decay Vertex Y [mm]", "Number of Hits");

  plot1DTr("n_radius", 500, -200, 200, "Radius [mm]", "Number of Hits");

  plot2DTr("n_xy_calo", 500,-150, 150, 500,-120, 120, "Calo Decay Vertex X [mm]", "Calo Decay Vertex Y [mm]");

  plot1DTr("n_mom", 500,0, 3500,"Momentum [MeV]","Number of Hits");

  plot1DTr("n_time", 500, 0, 126000,"Time [ns]","Number of Hits");

  plot1DTr("n_run", 71, 15921, 15992, "Run Number", "Number of Hits");

  plot1DTr("n_energy",500, 0, 4000,"Energy in Calo [MeV]","Number of Hits");

  plot2DTr("n_xy_calo_fit", 500, -150, 150, 500, -120, 120,"Calo Decay Vertex X [mm]","Calo Decay Vertex Y [mm]");  

}

// ===== ===== ===== ===== ===== 

void Plotter::Run() {
  
  //loop over the clusterTracker/tracker tree:
  while( NextClTrTrEvent() ) {
    
    // Define run number
    int run = ctt -> runNum;
  
    //====Volume cut, P value cut, Momentum cut at 2.3 GeV====

    if(ctt->hitVolume == true) continue;
    if(ctt->trackPValue < 0.05) continue;

    //====Time difference===
    const double dt = ctt->trackTimeDiff;
    
    //====Calo Energy
    const double E = ctt->clusterE;

    //====E/p====
    const double logEop = log(ctt->clusterEoverP);

    //classify the tracks:
    bool lostMu=false;

    // See https://gm2-docdb.fnal.gov/cgi-bin/private/RetrieveFile?docid=16402&filename=Nota5_LostMuons.pdf&version=3
    // Sudeshna's selection 100 < E < 500 [MeV]. "remove low energy glancing muons and low energy positrons that fake MIPs"
    const double p = ctt -> trackMomentum;
    if(100 < E && E < 500 && p > 2300 ) lostMu=true;
  
    const double caloX_raw = ctt->clusterX;
    const double caloY_raw = ctt->clusterY;
    const double caloX = 112.5 - 25*(caloX_raw);
    const double caloY = SetCaloY(ctt->clusterCaloNum, caloY_raw);
    // -75 + 25*(caloY_raw); //SetCaloY(ctt->clusterCaloNum, caloY_raw);

    //====Calo Decay Vertices====
    double cTrX = ctt->caloVertexPosX;
    double cTrY = ctt->caloVertexPosY;

    //====Tracker decay vertices====
    double TrX = ctt->decayVertexPosX; 
    double TrY = ctt->decayVertexPosY;
    double TrZ = ctt->decayVertexPosZ;

    //====Tracker decay times, ns====
    double t = (ctt -> decayTime);
    //====cut 30 us, completely wipes out lost muons====
    // if (t < 30000) continue;    

    //====Decay radii, tracker and calo====
    const double magicRadius = 7112; //[mm]
    const double dX = caloX - cTrX;
    const double dY = caloY - cTrY;
    const double cR = sqrt(dX*dX + dY*dY);
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

    // Check which cal crystal gets hit:
    const int caloNum = CaloNum(caloX_raw, caloY_raw);
    if(caloNum < -0.01) continue;

    // Hard cut on radius to get rid of low tail

    if (R > 200 || R < -200) continue;
    
    // dR Match

    const double dR = sqrt(dX*dX + dY*dY);
  
    // dt and dR cut

    if (dR > 30) continue;
    if ( dt < -10 && dt > 6) continue;

    if ( lostMu ) { 

      Fill2DTr("mu_EoP",p,E);
      Fill2DTr("mu_pt",t,p);
      Fill1DTr("mu_y_track", TrY);
      Fill1DTr("mu_run",run);
      Fill1DTr("mu_energy",E);
      Fill1DTr("mu_radius",R);
      Fill2DTr("mu_xy_calo",cTrX,cTrY);
      Fill1DTr("mu_mom",p);
      Fill1DTr("mu_time",t);
      Fill2DTr("mu_xy_calo_fit",cTrX,cTrY);

    } 

    if ( !lostMu ) {

      Fill2DTr("n_EoP",p,E);
      Fill2DTr("n_pt",t,p);
      Fill1DTr("n_y_track", TrY);
      Fill1DTr("n_run",run);
      Fill1DTr("n_energy",E);
      Fill1DTr("n_radius",R);
      Fill2DTr("n_xy_calo",cTrX,cTrY);
      Fill1DTr("n_mom",p);
      Fill1DTr("n_time",t);
      Fill2DTr("n_xy_calo_fit",cTrX,cTrY);
    }
	
 
  } //event loop



  delete ctt;
  return;

}





