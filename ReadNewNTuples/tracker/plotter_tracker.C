//Simple code to make plots from the Europa OmegaA ntuples
// gavin.hesketh@ucl.ac.uk
// modified by: samuel.grant.18@ucl.ac.uk

//to use a particular branch, make sure it is uncommented in Reader.C
//branch variables are listed in Reader.h


//Just going with one region



#define Plotter_C
#include "Plotter.h"
#include "TMath.h"


int CaloNum(int caloX, int caloY) {
  return caloX+9*caloY;
}

void Plotter::InitTrees(TString input_file) {
  //initialise the trees you want to read
  //then enable the relevant branches in Reader.C
  // ctt = new clusterTrackerTrackReader(input_file, "clusterTracker");  
  // cr = new clusterTrackerClusterReader(input_file);  
  // cl = new clusterReader(input_file);  
   tr = new trackerReader(input_file);  
  //am = new allmuonsReader(input_file);  
}

void Plotter::InitHistos() {

    plot1D("time",500,0,4200*150,"Decay Time [ns]","N");
    plot1D("vertical", 160, -120, 120, "Vertical Decay Vertex [mm]", "N");
    plot1D("radial", 400, -200, 200, "Radial Decay Vertex [mm]", "N");
    plot1D("vertical60", 80, -60, 60, "Vertical Decay Vertex [mm]", "N");
    plot1D("radial60", 120, -60, 60, "Radial Decay Vertex [mm]", "N");
    plot1D("momentum",100, 0, 3500, "Track Momentum [MeV]", "N");
    plot2D("xz",100,-8000,8000,100,-8000,8000,"Track Decay Vertex X [mm]","Track Decay Vertex Z [mm]");

    //  for (int stn = 12 ; stn < 19; stn = stn + 6 ) {
    // BUG WITH STATION 12
    const int stn = 18;

    plot1D("St"+std::to_string(stn)+"_time",500,0,4200*150,"Decay Time [ns]","N");
    plot1D("St"+std::to_string(stn)+"_vertical", 160, -120, 120, "Vertical Decay Vertex [mm]", "N");
    plot1D("St"+std::to_string(stn)+"_radial", 400, -200, 200, "Radial Decay Vertex [mm]", "N");
    plot1D("St"+std::to_string(stn)+"_vertical60", 80, -60, 60, "Vertical Decay Vertex [mm]", "N");
    plot1D("St"+std::to_string(stn)+"_radial60", 120, -60, 60, "Radial Decay Vertex [mm]", "N");
    plot1D("St"+std::to_string(stn)+"_momentum",100, 0, 3500, "Track Momentum [MeV]", "N");
    plot2D("St"+std::to_string(stn)+"_xz",100,-8000,8000,100,-8000,8000,"Track Decay Vertex X [mm]","Track Decay Vertex Z [mm]");

    // }
}


//=========================================================

//loop over the entries in the tree, making plots:

void Plotter::Run() {


  //loop over the clusterTracker/tracker tree:
  while( NextTrEvent() ) {

    if(tr->passTrackQuality == false) continue;
    // if(tr->passVertexQuality == false) continue;
    if(tr->passCandidateQuality == false) continue;
    if(tr->hitVolume == true) continue;
      
    const int stn = tr->station;

    const double t = tr->decayTime;
    const double p = tr->trackMomentum;
    const double trX = tr->decayVertexPosX;
    const double trY = tr->decayVertexPosY;
    const double trZ = tr->decayVertexPosZ;
     
    // Set beam centre to zero
    const int magicRadius = 7112;
    double R = sqrt(trX*trX+trZ*trZ) - magicRadius;

    // Clean up zeros
    if (trX == 0 || trY == 0 || trZ == 0) continue;
    // Hard cut on radius to get rid of low tail
    if (R > 200 || R < -200) continue;

    Fill1D("time",t);
    Fill1D("radial",R);
    Fill1D("vertical",trY);
    Fill1D("radial60",R);
    Fill1D("vertical60",trY);
    Fill1D("momentum",p);   
    Fill2D("xz",trX,trZ);  

    if (stn!=18) continue;

    Fill1D("St"+std::to_string(stn)+"_time",t);
    Fill1D("St"+std::to_string(stn)+"_vertical",trY);
    Fill1D("St"+std::to_string(stn)+"_radial",R);
    Fill1D("St"+std::to_string(stn)+"_vertical60",trY);
    Fill1D("St"+std::to_string(stn)+"_radial60",R);
    Fill1D("St"+std::to_string(stn)+"_momentum",p);   
    Fill2D("St"+std::to_string(stn)+"_xz",trX,trZ);    
    
  }

  delete tr;
  return;

}
