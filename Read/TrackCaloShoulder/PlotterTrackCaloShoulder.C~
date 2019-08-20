//////////////////////////////
// Sam Grant                //
// August 2019              //
// Read allmuons branch     //
//////////////////////////////

#define Plotter_C
#include "Plotter.h"
#include "TMath.h"

int CaloNum(int caloX, int caloY) {
  return caloX+9*caloY;
}

void Plotter::InitTrees(TString input_file) {
  am = new allmuonsReader(input_file);  
}

void Plotter::InitHistos() {

  const double vRes = 1.5;
  const double rRes = 1.0;
  const double pRes = 0.03;

  plot1D("dR",200,0,70,"dR [mm]","Entries");

  // k=0, all particles; k=1, non lost muons; k=2, lost muons
  for (int k(0); k<3; k++) {

    plot1D("time_"+std::to_string(k),150*10,0,4200*150,"Decay Time [ns]","Entries");
    plot1D("vertical_"+std::to_string(k), (120*2)/vRes,-120,120,"Vertical Decay Vertex [mm]","Entries");
    plot1D("radial_"+std::to_string(k),(200*2)/rRes,-200,200,"Radial Decay Vertex [mm]","Entries");
    plot1D("vertical60_"+std::to_string(k),(60*2)/vRes,-60,60,"Vertical Decay Vertex [mm]","Entries");
    plot1D("radial60_"+std::to_string(k),(60*2)/rRes,-60,60,"Radial Decay Vertex [mm]","Entries");
    plot1D("p_"+std::to_string(k),3500*pRes,0,3500,"Track Momentum [MeV]","Entries");
    plot1D("logEop_"+std::to_string(k),200,-3.5,1,"Log(E/p)","Entries");
    plot2D("xz_"+std::to_string(k),1000,-8000,1000,1000,-8000,8000,"Track Decay Vertex X [mm]","Track Decay Vertex Z [m]");
    plot2D("Evp_"+std::to_string(k),1000,0,4000,1000,0,4000,"Track Momentum [MeV]","Cluster Energy [MeV]");

    for (int stn(12); stn<19; stn=stn+6) {

      plot1D("St"+std::to_string(stn)+"_time_"+std::to_string(k),150*10,0,4200*150,"Decay Time [ns]","Entries");
      plot1D("St"+std::to_string(stn)+"_vertical_"+std::to_string(k),(120*2)/vRes,-120,120,"Vertical Decay Vertex [mm]","Entries");
      plot1D("St"+std::to_string(stn)+"_radial_"+std::to_string(k), (200*2)/rRes,-200,200,"Radial Decay Vertex [mm]","Entries");
      plot1D("St"+std::to_string(stn)+"_vertical60_"+std::to_string(k),(60*2)/vRes,-60,60,"Vertical Decay Vertex [mm]","Entries");
      plot1D("St"+std::to_string(stn)+"_radial60_"+std::to_string(k),(60*2)/rRes,-60,60,"Radial Decay Vertex [mm]","Entries");
      plot1D("St"+std::to_string(stn)+"_p_"+std::to_string(k),3500*pRes,0,3500,"Track Momentum [MeV]","Entries");
      plot2D("St"+std::to_string(stn)+"_xz_"+std::to_string(k),1000,-8000,1000,1000,-8000,8000,"Track Decay Vertex X [mm]","Track Decay Vertex Z [mm]");
      plot2D("St"+std::to_string(stn)+"_Evp_"+std::to_string(k),1000,0,4000,1000,0,4000,"Track Momentum [MeV]","Cluster Energy [MeV]");
      plot1D("St"+std::to_string(stn)+"_dt_"+std::to_string(k),200,-15,15,"dt [ns]","Entries");
      plot1D("St"+std::to_string(stn)+"_logEop_"+std::to_string(k),200,-3.5,1,"Log(E/p)","Entries");
      
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
       //       if(am->trkPassCandidateQuality[i] == false) continue;
      
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
     
      int k = 3;
      if (!lostMu) k=1;
      if (lostMu) k=2;
      if(k==3) std::cout<<"**Error**"<<std::endl;
      
	Fill1D("dR",dR);

	Fill1D("time_0",t);
	Fill1D("vertical_0",trY);
	Fill1D("radial_0",R);
	Fill1D("vertical60_0",trY);
	Fill1D("radial60_0",R);
	Fill1D("p_0",p);
	Fill2D("xz_0",trX,trZ);  
	Fill2D("Evp_0",p,E);
	Fill1D("logEop_0",logEop);
   
	Fill1D("time_"+std::to_string(k),t);
	Fill1D("vertical_"+std::to_string(k),trY);
	Fill1D("radial_"+std::to_string(k),R);
	Fill1D("vertical60_"+std::to_string(k),trY);
	Fill1D("radial60_"+std::to_string(k),R);
	Fill1D("p_"+std::to_string(k),p);   
	Fill2D("xz_"+std::to_string(k),trX,trZ);  
	Fill2D("Evp_"+std::to_string(k),p,E);
	Fill1D("logEop_"+std::to_string(k),logEop);
   
	Fill1D("St"+std::to_string(trkSt)+"_time_0",t);
	Fill1D("St"+std::to_string(trkSt)+"_vertical_0",trY);
	Fill1D("St"+std::to_string(trkSt)+"_radial_0",R);
	Fill1D("St"+std::to_string(trkSt)+"_vertical60_0",trY);
	Fill1D("St"+std::to_string(trkSt)+"_radial60_0",R);
	Fill1D("St"+std::to_string(trkSt)+"_p_0",p);   
	Fill2D("St"+std::to_string(trkSt)+"_xz_0",trX,trZ);  
	Fill2D("St"+std::to_string(trkSt)+"_Evp_0",p,E);
	Fill1D("St"+std::to_string(trkSt)+"_logEop_0",logEop);
	Fill1D("St"+std::to_string(trkSt)+"_dt_0",dt);
	
	Fill1D("St"+std::to_string(trkSt)+"_time_"+std::to_string(k),t);
	Fill1D("St"+std::to_string(trkSt)+"_vertical_"+std::to_string(k),trY);
	Fill1D("St"+std::to_string(trkSt)+"_radial_"+std::to_string(k),R);
	Fill1D("St"+std::to_string(trkSt)+"_vertical60_"+std::to_string(k),trY);
	Fill1D("St"+std::to_string(trkSt)+"_radial60_"+std::to_string(k),R);
	Fill1D("St"+std::to_string(trkSt)+"_p_"+std::to_string(k),p);   
	Fill2D("St"+std::to_string(trkSt)+"_xz_"+std::to_string(k),trX,trZ);  
	Fill2D("St"+std::to_string(trkSt)+"_Evp_"+std::to_string(k),p,E);
	Fill1D("St"+std::to_string(trkSt)+"_logEop_"+std::to_string(k),logEop);
	Fill1D("St"+std::to_string(trkSt)+"_dt_"+std::to_string(k),dt);
	
      }
    
    }
    delete am;
    return;

  }
