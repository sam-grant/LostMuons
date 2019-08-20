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
  //  tr = new trackerReader(input_file);  
  am = new allmuonsReader(input_file);  
}

void Plotter::InitHistos() {

  //  const int stn = 18;

  for (int stn = 13; stn < 20 ; stn = stn + 6 ) {
    for (int prt = 0 ; prt < 3 ; prt++ ) {


      plot2D("St"+std::to_string(stn)+"_xz_"+std::to_string(prt),100,-8000,8000,100,-8000,8000,"Track Decay Vertex X [mm]","Track Decay Vertex Z [mm]");

    }
  }

}

//=========================================================

//loop over the entries in the tree, making plots:

void Plotter::Run() {

  //loop over the clusterTracker/tracker tree:
  while( NextallmuonsEvent() ) {

    //loop over the matches in this event:
    for(int i=0; i<am->nmatches; i++) {

       // Apply quality cuts
       if(am->decayHitVolume[i] == true) continue;
       if(am->trkPassTrackQuality[i] == false) continue;
       //  if(am->trkPassVertexQuality[i] == false) continue;
       if(am->trkPassCandidateQuality[i] == false) continue;
      
      // Define some variables
      const int caloSt = am->cluCaloNum[i];
      const double dt = am->Tdiff[i];
      const int trkSt = am->trkStationNum[i];
      const double trX = am->decayvX[i];
      const double trY = am->decayvY[i];
      const double trZ = am->decayvZ[i];
      const double p = sqrt(am->trkMomX[i]*am->trkMomX[i] + am->trkMomY[i]*am->trkMomY[i] + am->trkMomZ[i]*am->trkMomZ[i]);
      const double t = am -> decayTime[i];
      const double E = am->cluEne[i];
      const double R = sqrt(trX*trX+trZ*trZ) - 7112;

      // Time difference
      if(caloSt==13){
	if (dt<-8 || dt>3 ) continue;
      }
      else {
	if(dt<-9 || dt>1) continue;        
      }

      //calo range from 0->8 in x (needs flipping), 0->5 in y
      //tracker range from -120 -> 100 in x, -50 to 50 in y
      // x scaling ~9/220=0.041; y scaling ~ 6/100 = 0.06
      
      const double caloX_raw = am->cluX[i];
      const double caloY_raw = am->cluY[i];
      //  const int xtalNum = CaloNum(caloX_raw, caloY_raw);
      //  if(xtalNum <-0.01) continue;
      
      const double caloX = 112.5 - 25*(caloX_raw);
      const double caloY = SetCaloY(caloSt, caloY_raw);

      // Radial Matching
      const double ctrX = am->vX[i];
      const double ctrY = am->vY[i];
      const double dX = caloX - ctrX;
      const double dY = caloY - ctrY;
      const double dR = sqrt(dX*dX + dY*dY);//- 7112;
      if(dR>30) continue;
  
      // Make particle selection     
      bool lostMu = false;
      const double logEop = log(am->EovP[i]);
      if(-3.2 < logEop && logEop < -2.4 && p > 2300) lostMu=true;
         
      // Clean up zeros
       if (trX == 0 || trY == 0 || trZ == 0) continue;
      // Hard cut on radius to get rid of low tail
       if (R > 200 || R < -200) continue;
 
     //========================== SNIP!
      int k;// = 0;
      if (!lostMu) {k=1;}
      else if (lostMu) {k=2;}

      Fill2D("St"+std::to_string(caloSt)+"_xz_0",trX,trZ);    
      Fill2D("St"+std::to_string(caloSt)+"_xz_"+std::to_string(k),trX,trZ);  
       
    }
    
 }
  delete am;
  return;

}
