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

  const int stn = 18;

    for (int prt = 0 ; prt < 3 ; prt++ ) {

      // time slices
      for (int t = 1; t < 6; t++) {

      plot1D("time_"+std::to_string(prt)+"_"+std::to_string(t),500,0,4200*150,"Decay Time [ns]","N");
      plot1D("vertical_"+std::to_string(prt)+"_"+std::to_string(t), 160, -120, 120, "Vertical Decay Vertex [mm]", "N");
      plot1D("radial_"+std::to_string(prt)+"_"+std::to_string(t), 400, -200, 200, "Radial Decay Vertex [mm]", "N");
      plot1D("vertical60_"+std::to_string(prt)+"_"+std::to_string(t), 80, -60, 60, "Vertical Decay Vertex [mm]", "N");
      plot1D("radial60_"+std::to_string(prt)+"_"+std::to_string(t), 120, -60, 60, "Radial Decay Vertex [mm]", "N");
      plot1D("momentum_"+std::to_string(prt)+"_"+std::to_string(t),100, 0, 3500, "Track Momentum [MeV]", "N");
      plot2D("xz_"+std::to_string(prt)+"_"+std::to_string(t),100,-8000,8000,100,-8000,8000,"Track Decay Vertex X [mm]","Track Decay Vertex Z [mm]");
      plot2D("Eop_"+std::to_string(prt)+"_"+std::to_string(t),100, 0, 3500, 33, 0, 4000,"Track Momentum [MeV]","Cluster Energy [MeV]");

      plot1D("St"+std::to_string(stn)+"_time_"+std::to_string(prt)+"_"+std::to_string(t),500,0,4200*150,"Decay Time [ns]","N");
      plot1D("St"+std::to_string(stn)+"_vertical_"+std::to_string(prt)+"_"+std::to_string(t), 160, -120, 120, "Vertical Decay Vertex [mm]", "N");
      plot1D("St"+std::to_string(stn)+"_radial_"+std::to_string(prt)+"_"+std::to_string(t), 400, -200, 200, "Radial Decay Vertex [mm]", "N");
      plot1D("St"+std::to_string(stn)+"_vertical60_"+std::to_string(prt)+"_"+std::to_string(t), 80, -60, 60, "Vertical Decay Vertex [mm]", "N");
      plot1D("St"+std::to_string(stn)+"_radial60_"+std::to_string(prt)+"_"+std::to_string(t), 120, -60, 60, "Radial Decay Vertex [mm]", "N");
      plot1D("St"+std::to_string(stn)+"_momentum_"+std::to_string(prt)+"_"+std::to_string(t),100, 0, 3500, "Track Momentum [MeV]", "N");
      plot2D("St"+std::to_string(stn)+"_xz_"+std::to_string(prt)+"_"+std::to_string(t),100,-8000,8000,100,-8000,8000,"Track Decay Vertex X [mm]","Track Decay Vertex Z [mm]");
      plot2D("St"+std::to_string(stn)+"_Eop_"+std::to_string(prt)+"_"+std::to_string(t),100, 0, 3500, 33, 0, 4000,"Track Momentum [MeV]","Cluster Energy [MeV]");
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
      const double t = am -> cluTime[i];
      
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
       //////
       if(t < 30000) continue;
 
     //========================== SNIP!
      int k;// = 0;
      if (!lostMu) {k=1;}
      if (lostMu) { k=2;}

      int slice;
      if (30000 < t && t < 130000) {slice=1;}
      else if (130000 < t && t < 230000) {slice=2;}
      else if (230000 < t && t < 330000) {slice=3;}
      else if (330000 < t && t < 430000) {slice=4;}
      else if (430000 < t && t < 530000) {slice=5;}
      else if (530000 < t && t < 630000) {slice=6;}


      Fill1D("time_0_"+std::to_string(slice),t);
      Fill1D("vertical_0_"+std::to_string(slice),trY);
      Fill1D("radial_0_"+std::to_string(slice),R);
      Fill1D("vertical60_0_"+std::to_string(slice),trY);
      Fill1D("radial60_0_"+std::to_string(slice),R);
      Fill1D("momentum_0_"+std::to_string(slice),p);   
      Fill2D("xz_0_"+std::to_string(slice),trX,trZ);  
      Fill2D("Eop_0_"+std::to_string(slice),p,E);
   
      Fill1D("time_"+std::to_string(k)+"_"+std::to_string(slice),t);
      Fill1D("vertical_"+std::to_string(k)+"_"+std::to_string(slice),trY);
      Fill1D("radial_"+std::to_string(k)+"_"+std::to_string(slice),R);
      Fill1D("vertical60_"+std::to_string(k)+"_"+std::to_string(slice),trY);
      Fill1D("radial60_"+std::to_string(k)+"_"+std::to_string(slice),R);
      Fill1D("momentum_"+std::to_string(k)+"_"+std::to_string(slice),p);   
      Fill2D("xz_"+std::to_string(k)+"_"+std::to_string(slice),trX,trZ);  
      Fill2D("Eop_"+std::to_string(k)+"_"+std::to_string(slice),p,E);
   
      if (trkSt!=18) continue;

      Fill1D("St"+std::to_string(trkSt)+"_time_0_"+std::to_string(slice),t);
      Fill1D("St"+std::to_string(trkSt)+"_vertical_0_"+std::to_string(slice),trY);
      Fill1D("St"+std::to_string(trkSt)+"_radial_0_"+std::to_string(slice),R);
      Fill1D("St"+std::to_string(trkSt)+"_vertical60_0_"+std::to_string(slice),trY);
      Fill1D("St"+std::to_string(trkSt)+"_radial60_0_"+std::to_string(slice),R);
      Fill1D("St"+std::to_string(trkSt)+"_momentum_0_"+std::to_string(slice),p);   
      Fill2D("St"+std::to_string(trkSt)+"_xz_0_"+std::to_string(slice),trX,trZ);  
      Fill2D("St"+std::to_string(trkSt)+"_Eop_0_"+std::to_string(slice),p,E);
           
      Fill1D("St"+std::to_string(trkSt)+"_time_"+std::to_string(k)+"_"+std::to_string(slice),t);
      Fill1D("St"+std::to_string(trkSt)+"_vertical_"+std::to_string(k)+"_"+std::to_string(slice),trY);
      Fill1D("St"+std::to_string(trkSt)+"_radial_"+std::to_string(k)+"_"+std::to_string(slice),R);
      Fill1D("St"+std::to_string(trkSt)+"_vertical60_"+std::to_string(k)+"_"+std::to_string(slice),trY);
      Fill1D("St"+std::to_string(trkSt)+"_radial60_"+std::to_string(k)+"_"+std::to_string(slice),R);
      Fill1D("St"+std::to_string(trkSt)+"_momentum_"+std::to_string(k)+"_"+std::to_string(slice),p);   
      Fill2D("St"+std::to_string(trkSt)+"_xz_"+std::to_string(k)+"_"+std::to_string(slice),trX,trZ);  
      Fill2D("St"+std::to_string(trkSt)+"_Eop_"+std::to_string(k)+"_"+std::to_string(slice),p,E);
       
    }
    
 }
  delete am;
  return;

}
