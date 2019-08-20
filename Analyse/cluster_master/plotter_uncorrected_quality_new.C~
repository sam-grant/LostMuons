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

  const double ymin = 0.71;
  const double ymax = 1.25;
  // const int fidMain = 0;
  //const int board = 0;
  for (int stn = 13; stn < 20 ; stn = stn + 6) {
    //  for (int board = 0; board < 3; board++) {
    // for (int fidMain = 0; fidMain < 3; fidMain++) {
    for (int fidXtal = 0; fidXtal < 3; fidXtal++) {

      /*	    plot2D("St"+std::to_string(stn)+"_E_vs_p_"+std::to_string(board)+"_"+std::to_string(fidMain)+"_"+std::to_string(fidXtal),20,0,4000,20,0,4000,"p [MeV]","E [MeV]");
		    plot2D("St"+std::to_string(stn)+"_Ep_vs_E_"+std::to_string(board)+"_"+std::to_string(fidMain)+"_"+std::to_string(fidXtal),20,0,4000,200,ymin,ymax,"E [MeV]","E/p");
		    plot2D("St"+std::to_string(stn)+"_Ep_vs_t_"+std::to_string(board)+"_"+std::to_string(fidMain)+"_"+std::to_string(fidXtal),150,0,5*126000,200,ymin,ymax,"t [ns]","E/p");
		    plot2D("St"+std::to_string(stn)+"_Ep_vs_t_early_"+std::to_string(board)+"_"+std::to_string(fidMain)+"_"+std::to_string(fidXtal),50,0,4200*50,200,ymin,ymax,"t [ns]", "E/p");
		    plot2D("St"+std::to_string(stn)+"_Ep_vs_p_"+std::to_string(board)+"_"+std::to_string(fidMain)+"_"+std::to_string(fidXtal),20,0,4000,200,ymin,ymax,"p [MeV]","E/p");
		    plot2D("St"+std::to_string(stn)+"_xy_calo_"+std::to_string(board)+"_"+std::to_string(fidMain)+"_"+std::to_string(fidXtal),60,-150,150,48,-120,120,"Calo Position X [mm]","Calo Position Y [mm]");
		    plot2D("St"+std::to_string(stn)+"_Ep_vs_x_"+std::to_string(board)+"_"+std::to_string(fidMain)+"_"+std::to_string(fidXtal),60,-150,150,200,ymin,ymax,"Calo Position X [mm]","E/p");
		    plot2D("St"+std::to_string(stn)+"_Ep_vs_y_"+std::to_string(board)+"_"+std::to_string(fidMain)+"_"+std::to_string(fidXtal),48,-120,120,200,ymin,ymax,"Calo Position Y [mm]","E/p");
		    plot2D("St"+std::to_string(stn)+"_Ep_vs_xtal_"+std::to_string(board)+"_"+std::to_string(fidMain)+"_"+std::to_string(fidXtal),54,0,53,200,ymin,ymax,"Crystal Number","E/p");
      */
      //	}
      // }
      // }
      //  }
      plot2D("St"+std::to_string(stn)+"_Ep_vs_xtal_"+std::to_string(fidXtal),54,-0.5,53.5,200,ymin,ymax,"Crystal Number","E/p");
      // plot2D("xy_calo_"+std::to_string(fidMain),200,-150,150,200,-120,120,"Calo Position X [mm]","Calo Position Y [mm]");
      //plot2D("Ep_vs_x_"+std::to_string(fidMain),60,-150,150,200,ymin,ymax,"Calo Position X [mm]","E/p");
      // plot2D("Ep_vs_y_"+std::to_string(fidMain),48,-120,120,200,ymin,ymax,"Calo Position Y [mm]","E/p");
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

      if(am->decayHitVolume[i] == true) continue;
    
      if( am->trkPvalue[i] < 0.05) continue;
      // require quality cut pass
      if(am->trkPassTrackQuality[i] == false) continue;
      
      double p = sqrt(am->trkMomX[i]*am->trkMomX[i] + am->trkMomY[i]*am->trkMomY[i] + am->trkMomZ[i]*am->trkMomZ[i]);
      // Fill2D("E_vs_p", p, am->cluEne[i]);
      
      const double logEop = log(am->EovP[i]);
      const double dt = am->Tdiff[i];
      //Fill1D("Eop", logEop);
      // Fill1D("dt", dt );

      const int caloSt = am->cluCaloNum[i];
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
      const int xtalNum = CaloNum(caloX_raw, caloY_raw);
      // cout << xtalNum << endl;
      //Convert to mm from calo number I believe
      const double caloX = 112.5 - 25*(caloX_raw);
      const double caloY = SetCaloY(caloSt, caloY_raw);
      // -75 + 25*(caloY_raw); //SetCaloY(ctt->clusterCaloNum, caloY_raw);
     
      // Tracker decay vertices
      double trX = am->vX[i];
      double trY = am->vY[i];

      //the raw distributions:
      //  Fill2D("xy_calo_raw", caloX_raw, caloY_raw);
      //Fill2D("xy_calo", caloX, caloY);
      //Fill2D("xy_track", trX, trY);
      
      const double dX = caloX - trX;
      const double dY = caloY - trY;
      const double dR = sqrt(dX*dX + dY*dY);
      
      if(dR>30) continue;

      double t = (am -> decayTime[i]);

      // Time cut 
      bool tcut = false;
      if (t > 30000) tcut = true;

      double E = am->cluEne[i];
      double Ep = E/p;
      //========================== SNIP!
    
  
      bool region[5] = {false};
      // All Tracks   
      region[0] = true;
      // Positrons 
      if(logEop>-0.3 && logEop<0.2 ) region[1]=true;
      // High Flux - Energy - All Tracsk
      if (1250 < E && E < 2250) region[2]=true;
      // High Flux - Energy - Positrons
      if(region[1]==true && region[2]==true) region[3]=true;   
      if(!region[3]) continue;
      //   Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_E_0_0_0",E,Ep);
      //  Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_p_0_0_0",p,Ep);
      int fidXtal;// = 0;
      if (FiducialXtal(trX, trY)) {fidXtal=1;}
      else if (!FiducialXtal(trX, trY)) {fidXtal=2;}

      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_xtal_0",xtalNum,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_xtal_"+std::to_string(fidXtal),xtalNum,Ep);
    

      //  Fill2D("xy_calo_"+std::to_string(fidMain),trX,trY);
      // Fill2D("Ep_vs_x_"+std::to_string(fidMain),trX,Ep);
      //Fill2D("Ep_vs_y_"+std::to_string(fidMain),trY,Ep);
      ////////////////////////////////////////////
      //int lowLifeXtal[22] = {0,9,10,11,14,15,18,19,20,23,24,27,30,31,34,35,36,39,40,43,44,45};
      // const int brd = 0;
      /*for (int i = 0 ; i < 22 ; i++ ) {
	if (xtalNum == lowLifeXtal[i]) {
	  brd = 1;
      	}
	else if (xtalNum != lowLifeXtal[i]) {
	  brd = 2;
	}
	else { 
	  cout << "Error, no crystal match! xtal = " << xtalNum << endl; 
	  }*/

      // const int fidMain = 0;
	//if (FiducialSam(trX, trY)) {fidMain=1;}
	// else if (!FiducialSam(trX, trY)) {fidMain=2;}
	
       //   int fidXtal;
       // if (FiducialSam(trX, trY)) {fidXtal=1;}
       //else if (!FiducialSam(trX, trY)) {fidXtal=2;}

      /* Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_E_0_0_0",E,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_p_0_0_0",p,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_t_0_0_0",t,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_t_early_0_0_0",t,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_xy_calo_0_0_0",trX,trY);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_x_0_0_0",trX,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_y_0_0_0",trY,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_xtal_0_0_0",xtalNum,Ep);

      Fill2D("St"+std::to_string(caloSt)+"_E_vs_p_0_0_"+std::to_string(fidXtal),p,E);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_E_"+std::to_string(brd)+"_"+std::to_string(fidMain)+"_"+std::to_string(fidXtal),E,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_p_"+std::to_string(brd)+"_"+std::to_string(fidMain)+"_"+std::to_string(fidXtal),p,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_t_"+std::to_string(brd)+"_"+std::to_string(fidMain)+"_"+std::to_string(fidXtal),t,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_t_early_"+std::to_string(brd)+"_"+std::to_string(fidMain)+"_"+std::to_string(fidXtal),t,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_xy_calo_"+std::to_string(brd)+"_"+std::to_string(fidMain)+"_"+std::to_string(fidXtal),trX,trY);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_x_"+std::to_string(brd)+"_"+std::to_string(fidMain)+"_"+std::to_string(fidXtal),trX,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_y_"+std::to_string(brd)+"_"+std::to_string(fidMain)+"_"+std::to_string(fidXtal),trY,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_xtal_"+std::to_string(brd)+"_"+std::to_string(fidMain)+"_"+std::to_string(fidXtal),xtalNum,Ep);


      */
      // Time cut 
      //  int tcut = 0;
      //if (t > 30000) tcut = true;
      /*
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_E_0_0_0",E,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_p_0_0_0",p,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_t_0_0_0",t,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_t_early_0_0_0",t,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_xy_calo_0_0_0",trX,trY);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_x_0_0_0",trX,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_y_0_0_0",trY,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_xtal_0_0_0",xtalNum,Ep);

      //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_E_"+std::to_string(brd)+"_0_0",E,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_p_"+std::to_string(brd)+"_0_0",p,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_t_"+std::to_string(brd)+"_0_0",t,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_t_early_"+std::to_string(brd)+"_0_0",t,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_xy_calo_"+std::to_string(brd)+"_0_0",trX,trY);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_x_"+std::to_string(brd)+"_0_0",trX,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_y_"+std::to_string(brd)+"_0_0",trY,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_xtal_"+std::to_string(brd)+"_0_0",xtalNum,Ep);

      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_E_0_"+std::to_string(fidMain)+"_0",E,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_p_0_"+std::to_string(fidMain)+"_0",p,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_t_0_"+std::to_string(fidMain)+"_0",t,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_t_early_0_"+std::to_string(fidMain)+"_0",t,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_xy_calo_0_"+std::to_string(fidMain)+"_0",trX,trY);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_x_0_"+std::to_string(fidMain)+"_0",trX,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_y_0_"+std::to_string(fidMain)+"_0",trY,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_xtal_0_"+std::to_string(fidMain)+"_0",xtalNum,Ep);

      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_E_0_0_"+std::to_string(fidXtal),E,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_p_0_0_"+std::to_string(fidXtal),p,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_t_0_0_"+std::to_string(fidXtal),t,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_t_early_0_0_"+std::to_string(fidXtal),t,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_xy_calo_0_0_"+std::to_string(fidXtal),trX,trY);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_x_0_0_"+std::to_string(fidXtal),trX,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_y_0_0_"+std::to_string(fidXtal),trY,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_xtal_0_0_"+std::to_string(fidXtal),xtalNum,Ep);

      //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_E_0_"+std::to_string(fidMain)+"_"+std::to_string(fidXtal),E,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_p_0_"+std::to_string(fidMain)+"_"+std::to_string(fidXtal),p,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_t_0_"+std::to_string(fidMain)+"_"+std::to_string(fidXtal),t,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_t_early_0_"+std::to_string(fidMain)+"_"+std::to_string(fidXtal),t,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_xy_calo_0_"+std::to_string(fidMain)+"_"+std::to_string(fidXtal),trX,trY);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_x_0_"+std::to_string(fidMain)+"_"+std::to_string(fidXtal),trX,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_y_0_"+std::to_string(fidMain)+"_"+std::to_string(fidXtal),trY,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_xtal_0_"+std::to_string(fidMain)+"_"+std::to_string(fidXtal),xtalNum,Ep);

      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_E_"+std::to_string(brd)+"_0_"+std::to_string(fidXtal),E,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_p_"+std::to_string(brd)+"_0_"+std::to_string(fidXtal),p,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_t_"+std::to_string(brd)+"_0_"+std::to_string(fidXtal),t,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_t_early_"+std::to_string(brd)+"_0_"+std::to_string(fidXtal),t,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_xy_calo_"+std::to_string(brd)+"_0_"+std::to_string(fidXtal),trX,trY);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_x_"+std::to_string(brd)+"_0_"+std::to_string(fidXtal),trX,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_y_"+std::to_string(brd)+"_0_"+std::to_string(fidXtal),trY,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_xtal_"+std::to_string(brd)+"_0_"+std::to_string(fidXtal),xtalNum,Ep);

      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_E_"+std::to_string(brd)+"_"+std::to_string(fidMain)+"_0",E,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_p_"+std::to_string(brd)+"_"+std::to_string(fidMain)+"_0",p,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_t_"+std::to_string(brd)+"_"+std::to_string(fidMain)+"_0",t,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_t_early_"+std::to_string(brd)+"_"+std::to_string(fidMain)+"_0",t,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_xy_calo_"+std::to_string(brd)+"_"+std::to_string(fidMain)+"_0",trX,trY);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_x_"+std::to_string(brd)+"_"+std::to_string(fidMain)+"_0",trX,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_y_"+std::to_string(brd)+"_"+std::to_string(fidMain)+"_0",trY,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_xtal_"+std::to_string(brd)+"_"+std::to_string(fidMain)+"_0",xtalNum,Ep);

      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

      Fill2D("St"+std::to_string(caloSt)+"_E_vs_p_"+std::to_string(brd)+"_"+std::to_string(fidMain)+"_"+std::to_string(fidXtal),p,E);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_E_"+std::to_string(brd)+"_"+std::to_string(fidMain)+"_"+std::to_string(fidXtal),E,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_p_"+std::to_string(brd)+"_"+std::to_string(fidMain)+"_"+std::to_string(fidXtal),p,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_t_"+std::to_string(brd)+"_"+std::to_string(fidMain)+"_"+std::to_string(fidXtal),t,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_t_early_"+std::to_string(brd)+"_"+std::to_string(fidMain)+"_"+std::to_string(fidXtal),t,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_xy_calo_"+std::to_string(brd)+"_"+std::to_string(fidMain)+"_"+std::to_string(fidXtal),trX,trY);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_x_"+std::to_string(brd)+"_"+std::to_string(fidMain)+"_"+std::to_string(fidXtal),trX,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_y_"+std::to_string(brd)+"_"+std::to_string(fidMain)+"_"+std::to_string(fidXtal),trY,Ep);
      Fill2D("St"+std::to_string(caloSt)+"_Ep_vs_xtal_"+std::to_string(brd)+"_"+std::to_string(fidMain)+"_"+std::to_string(fidXtal),xtalNum,Ep);
      */
      }
   
     
    }
    
  //}

  return;

}
