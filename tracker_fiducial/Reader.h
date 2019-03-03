//Simple code to read the Europa OmegaA ntuples
// gavin.hesketh@ucl.ac.uk
//
// You shouldn't have to edit this, except to declare new branches
//    or a reader for a new kind of tree.


#ifndef Reader_h
#define Reader_h

#include "TString.h"
#include <iostream>
#include <vector>
#include "TMatrixD.h"

class TBranch;
class TTree;


//====================================================================
//=== Generic tree reader
//====================================================================
//this has some general functions to open the file, load next event, etc
class Reader {

 public: 
  Reader(TString filename, TString folder, TString treename);
  ~Reader(){};
  bool NextEvent();
  long Entries() {return nentries_;}

 protected:
  template<class VAR>
  void LoadBranch(TString name, VAR &var, TBranch *&branch);
  virtual void Init(){};
  
  TTree          *fChain;   //!pointer to the analyzed TTree or TChain
  int           fCurrent; //!current Tree number in a TChain
  long nentries_;
  long jentry_;
};



//====================================================================
//=== clusterTracker Track reader
//====================================================================
//contains the branches specific to the clusterTracker track tree

class clusterTrackerTrackReader : public Reader {

 public :
 clusterTrackerTrackReader(TString filename, TString treename="clusterTracker"):
  Reader(filename, treename, "tracker"){
    Init();
    std::cout<<"==================================="<<std::endl<<std::endl;
    
  }


   // Declaration of leaf types
   Int_t           runNum;
   Int_t           subRunNum;
   Int_t           eventNum;
   Int_t           bunchNum;
   Int_t           clusterFillNum;
   Int_t           clusterCaloNum;
   Int_t           clusterIslandNum;
   Double_t        clusterX;
   Double_t        clusterY;
   Int_t           clusterHits;
   Double_t        clusterTime;
   Double_t        clusterE;
   Double_t        trackMomentum;
   Double_t        trackMomentumX;
   Double_t        trackMomentumY;
   Double_t        trackMomentumZ;
   Double_t        trackMomentumPhi;
   Double_t        trackMomentumTheta;
   Double_t        caloVertexPosX;
   Double_t        caloVertexPosY;
   Double_t        caloVertexPosZ;
   Double_t        caloVertexPosPhi;
   Double_t        caloVertexPosTheta;
   Double_t        decayVertexPosX;
   Double_t        decayVertexPosY;
   Double_t        decayVertexPosZ;
   Double_t        decayVertexMomX;
   Double_t        decayVertexMomY;
   Double_t        decayVertexMomZ;
   Double_t        clusterEoverP;
   Double_t        trackTimeDiff;
   Double_t        decayTime;
   Bool_t          hitVolume;
   Double_t        trackPValue;
   Int_t           station;

   
 private:

    // List of branches
   TBranch        *b_runNum;   //!
   TBranch        *b_subRunNum;   //!
   TBranch        *b_eventNum;   //!
   TBranch        *b_bunchNum;   //!
   TBranch        *b_clusterFillNum;   //!
   TBranch        *b_clusterCaloNum;   //!
   TBranch        *b_clusterIslandNum;   //!
   TBranch        *b_clusterX;   //!
   TBranch        *b_clusterY;   //!
   TBranch        *b_clusterHits;   //!
   TBranch        *b_clusterTime;   //!
   TBranch        *b_clusterE;   //!
   TBranch        *b_trackMomentum;   //!
   TBranch        *b_trackMomentumX;   //!
   TBranch        *b_trackMomentumY;   //!
   TBranch        *b_trackMomentumZ;   //!
   TBranch        *b_trackMomentumPhi;   //!
   TBranch        *b_trackMomentumTheta;   //!
   TBranch        *b_caloVertexPosX;   //!
   TBranch        *b_caloVertexPosY;   //!
   TBranch        *b_caloVertexPosZ;   //!
   TBranch        *b_caloVertexPosPhi;   //!
   TBranch        *b_caloVertexPosTheta;   //!
   TBranch        *b_decayVertexPosX;   //!
   TBranch        *b_decayVertexPosY;   //!
   TBranch        *b_decayVertexPosZ;   //!
   TBranch        *b_decayVertexMomX;   //!
   TBranch        *b_decayVertexMomY;   //!
   TBranch        *b_decayVertexMomZ;   //!
   TBranch        *b_clusterEoverP;   //!
   TBranch        *b_trackTimeDiff;   //!
   TBranch        *b_decayTime;   //!
   TBranch        *b_hitVolume;   //!
   TBranch        *b_trackPValue;   //!
   TBranch        *b_station;   //!
   
   void Init();

   //  TMatrixD w2t(3,3);
   //  TMatrixD w2c(3,3);

};


//====================================================================
//=== clusterTracker cluster reader
//====================================================================


class clusterTrackerClusterReader : public Reader {

 public :
 clusterTrackerClusterReader(TString filename):
  Reader(filename, "clusterTracker", "clusters"){
    Init();
    std::cout<<"==================================="<<std::endl<<std::endl;
      
  }

  // Declaration of leaf types
   Int_t           runNum;
   Int_t           subRunNum;
   Int_t           eventNum;
   Int_t           bunchNum;
   Int_t           islandNum1;
   Int_t           islandNum2;
   Int_t           islandNum3;
   Int_t           calo1;
   Int_t           calo2;
   Int_t           calo3;
   Int_t           xtal1;
   Int_t           xtal2;
   Int_t           xtal3;
   Double_t        x1;
   Double_t        x2;
   Double_t        x3;
   Double_t        y1;
   Double_t        y2;
   Double_t        y3;
   Double_t        time1;
   Double_t        energy2;
   Double_t        time2;
   Double_t        energy1;
   Double_t        energy3;
   Double_t        time3;
   Double_t        energy1_xtal;
   Double_t        Tcoin21;
   Double_t        deltaE21;
   Double_t        Tcoin31;
   Double_t        deltaE31;
   Double_t        Tcoin32;
   Double_t        deltaE32;
   Int_t           dcoin;
   Int_t           tcoin;
   Int_t           nHit1;
   Int_t           nHit2;
   Int_t           nHit3;
   Double_t        dE_low;
   Double_t        dE_high;
   Double_t        dT_low;
   Double_t        dT_high;
   Double_t        T_spl;
  
  
 private:


   // List of branches
   TBranch        *b_runNum;   //!
   TBranch        *b_subRunNum;   //!
   TBranch        *b_eventNum;   //!
   TBranch        *b_bunchNum;   //!
   TBranch        *b_islandNum1;   //!
   TBranch        *b_islandNum2;   //!
   TBranch        *b_islandNum3;   //!
   TBranch        *b_calo1;   //!
   TBranch        *b_calo2;   //!
   TBranch        *b_calo3;   //!
   TBranch        *b_xtal1;   //!
   TBranch        *b_xtal2;   //!
   TBranch        *b_xtal3;   //!
   TBranch        *b_x1;   //!
   TBranch        *b_x2;   //!
   TBranch        *b_x3;   //!
   TBranch        *b_y1;   //!
   TBranch        *b_y2;   //!
   TBranch        *b_y3;   //!
   TBranch        *b_time1;   //!
   TBranch        *b_time2;   //!
   TBranch        *b_time3;   //!
   TBranch        *b_energy1_xtal;   //!
   TBranch        *b_energy1;   //!
   TBranch        *b_energy2;   //!
   TBranch        *b_energy3;   //!
   TBranch        *b_deltaE21;   //!
   TBranch        *b_deltaE31;   //!
   TBranch        *b_deltaE32;   //!
   TBranch        *b_Tcoin21;   //!
   TBranch        *b_Tcoin31;   //!
   TBranch        *b_Tcoin32;   //!
   TBranch        *b_dcoin;   //!
   TBranch        *b_tcoin;   //!
   TBranch        *b_nHit1;   //!
   TBranch        *b_nHit2;   //!
   TBranch        *b_nHit3;   //!
   TBranch        *b_dE_low;   //!
   TBranch        *b_dE_high;   //!
   TBranch        *b_dT_low;   //!
   TBranch        *b_dT_high;   //!
   TBranch        *b_T_spl;   //!

   void Init();

};


//====================================================================
//=== cluster (clusterTree/clusters) reader
//====================================================================
//contains the branches specific to the clusterTree/cluster tree

class clusterReader : public Reader {

 public :
 clusterReader(TString filename):
  Reader(filename, "clusterTree", "clusters"){
    Init();
    std::cout<<"==================================="<<std::endl<<std::endl;
  }

  // Declaration of leaf types
   Double_t        energy;
   Double_t        time;
   Double_t        x;
   Double_t        y;
   std::vector<double>  *xtalNums;
   std::vector<double>  *xtalEnergies;
   std::vector<double>  *xtalTimes;
   UInt_t          nHit;
   UInt_t          caloNum;
   UInt_t          xtalNum;
   UInt_t          islandNum;
   UInt_t          eventNum;
   UInt_t          bunchNum;
   UInt_t          midasSerialNum;
   UInt_t          subRunNum;
   UInt_t          runNum;

 private:

   // List of branches
   TBranch        *b_energy;   //!
   TBranch        *b_time;   //!
   TBranch        *b_x;   //!
   TBranch        *b_y;   //!
   TBranch        *b_xtalNums;   //!
   TBranch        *b_xtalEnergies;   //!
   TBranch        *b_xtalTimes;   //!
   TBranch        *b_nHit;   //!
   TBranch        *b_caloNum;   //!
   TBranch        *b_xtalNum;   //!
   TBranch        *b_islandNum;   //!
   TBranch        *b_eventNum;   //!
   TBranch        *b_bunchNum;   //!
   TBranch        *b_midasSerialNum;   //!
   TBranch        *b_subRunNum;   //!
   TBranch        *b_runNum;   //
   
   void Init();

};


//====================================================================
//=== tracker (trackerNTup/tracker) reader
//====================================================================
//contains the branches specific to the trackerNtup/tracker tree

class trackerReader : public Reader {

 public :
 trackerReader(TString filename):
  Reader(filename, "trackerNTup", "tracker"){
    Init();
    std::cout<<"==================================="<<std::endl<<std::endl;
  }

  // Declaration of leaf types
   Int_t           runNum;
   Int_t           subRunNum;
   Int_t           eventNum;
   Double_t        trackMomentum;
   Double_t        trackMomentumX;
   Double_t        trackMomentumY;
   Double_t        trackMomentumZ;
   Double_t        decayVertexPosX;
   Double_t        decayVertexPosY;
   Double_t        decayVertexPosZ;
   Double_t        decayVertexMomX;
   Double_t        decayVertexMomY;
   Double_t        decayVertexMomZ;
   Double_t        caloVertexPosX;
   Double_t        caloVertexPosY;
   Double_t        caloVertexPosZ;
   Double_t        caloVertexMomX;
   Double_t        caloVertexMomY;
   Double_t        caloVertexMomZ;
   Double_t        trackT0;
   Double_t        time;
   Double_t        decayTime;
   Bool_t          hitVolume;
   Double_t        trackPValue;
   Int_t           station;

 private:
   
   // List of branches
   TBranch        *b_runNum;   //!
   TBranch        *b_subRunNum;   //!
   TBranch        *b_eventNum;   //!
   TBranch        *b_trackMomentum;   //!
   TBranch        *b_trackMomentumX;   //!
   TBranch        *b_trackMomentumY;   //!
   TBranch        *b_trackMomentumZ;   //!
   TBranch        *b_decayVertexPosX;   //!
   TBranch        *b_decayVertexPosY;   //!
   TBranch        *b_decayVertexPosZ;   //!
   TBranch        *b_decayVertexMomX;   //!
   TBranch        *b_decayVertexMomY;   //!
   TBranch        *b_decayVertexMomZ;   //!
   TBranch        *b_caloVertexPosX;   //!
   TBranch        *b_caloVertexPosY;   //!
   TBranch        *b_caloVertexPosZ;   //!
   TBranch        *b_caloVertexMomX;   //!
   TBranch        *b_caloVertexMomY;   //!
   TBranch        *b_caloVertexMomZ;   //!
   TBranch        *b_trackT0;   //!
   TBranch        *b_time;   //!
   TBranch        *b_decayTime;   //!
   TBranch        *b_hitVolume;   //!
   TBranch        *b_trackPValue;   //!
   TBranch        *b_station;   //!
   
   void Init();

};



#endif
