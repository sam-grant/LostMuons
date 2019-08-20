//////////////////////////////
// Sam Grant                //
// August 2019              //
// Read tracker branch      //
//////////////////////////////

#define Plotter_C
#include "Plotter.h"
#include "TMath.h"

int CaloNum(int caloX, int caloY) {
  return caloX+9*caloY;
}

void Plotter::InitTrees(TString input_file) {
   tr = new trackerReader(input_file);  
}

void Plotter::InitHistos() {

  const double vRes = 1.5;
  const double rRes = 1.0;
  const double pRes = 0.03;
  
  plot1D("time",150*10,0,4200*150,"Decay Time [ns]","Entries");
  plot1D("vertical", (120*2)/vRes, -120, 120, "Vertical Decay Vertex [mm]", "Entries");
  plot1D("radial", (200*2)/rRes, -200, 200, "Radial Decay Vertex [mm]", "Entries");
  plot1D("vertical60", (60*2)/vRes, -60, 60, "Vertical Decay Vertex [mm]", "Entries");
  plot1D("radial60", (60*2)/rRes, -60, 60, "Radial Decay Vertex [mm]", "Entries");
  plot1D("p",3500*pRes, 0, 3500, "Track Momentum [MeV]", "Entries");
  plot2D("xz",1000,-8000,1000,1000,-8000,8000,"Track Decay Vertex X [mm]","Track Decay Vertex Z [mm]");

  for (int stn(12); stn < 19; stn = stn + 6 ) {
    
    plot1D("St"+std::to_string(stn)+"_time",150*10,0,4200*150,"Decay Time [ns]","Entries");
    plot1D("St"+std::to_string(stn)+"_vertical", (120*2)/vRes, -120, 120, "Vertical Decay Vertex [mm]", "Entries");
    plot1D("St"+std::to_string(stn)+"_radial", (200*2)/rRes, -200, 200, "Radial Decay Vertex [mm]", "Entries");
    plot1D("St"+std::to_string(stn)+"_vertical60", (60*2)/vRes, -60, 60, "Vertical Decay Vertex [mm]", "Entries");
    plot1D("St"+std::to_string(stn)+"_radial60", (60*2)/rRes, -60, 60, "Radial Decay Vertex [mm]", "Entries");
    plot1D("St"+std::to_string(stn)+"_p",3500*pRes, 0, 3500, "Track Momentum [MeV]", "Entries");
    plot2D("St"+std::to_string(stn)+"_xz",1000,-8000,8000,1000,-8000,8000,"Track Decay Vertex X [mm]","Track Decay Vertex Z [mm]");

  }
}

//=========================================================

//loop over the entries in the tree, making plots:

void Plotter::Run() {
  //    int count = 0;
  //loop over the clusterTracker/tracker tree:
  while( NextTrEvent() ) {

    if(tr->passTrackQuality == false) continue;
    if(tr->hitVolume == true) continue;
      
    const int stn = tr->station;
    //    std::cout<<"Stn : "<<stn<<std::endl;
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
    //    if (R > 200 || R < -200) continue;
    
    //    if(stn==18) continue;
    
    Fill1D("time",t);
    Fill1D("radial",R);
    Fill1D("vertical",trY);
    Fill1D("radial60",R);
    Fill1D("vertical60",trY);
    Fill1D("p",p);
    Fill2D("xz",trX,trZ);

    Fill1D("St"+std::to_string(stn)+"_time",t);
    Fill1D("St"+std::to_string(stn)+"_vertical",trY);
    Fill1D("St"+std::to_string(stn)+"_radial",R);
    Fill1D("St"+std::to_string(stn)+"_vertical60",trY);
    Fill1D("St"+std::to_string(stn)+"_radial60",R);
    Fill1D("St"+std::to_string(stn)+"_p",p);
    Fill2D("St"+std::to_string(stn)+"_xz",trX,trZ);

    //  std::cout<<"count : "<<count<<std::endl;
    // count++;
  }

  delete tr;
  return;

}
