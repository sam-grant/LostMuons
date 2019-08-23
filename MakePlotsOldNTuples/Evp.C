// Marco to muon cut
// Sam Grant - Jan 2019

#include <iostream>
#include "TFile.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TLegend.h"
#include "CMAP.h"

using namespace std;


int main() {

  string fname2D = "Plots/EvpLostMuons.png";
  string inputName = "TrackCluster";

    TFile *input = TFile::Open(("../ReadOldNTuples/"+inputName+"/RootFiles/"+inputName+".root").c_str());

    TH2D *n2D = (TH2D*)input->Get("Evp_2");
    n2D->GetXaxis()->SetLimits(0,4);//;
    n2D->GetYaxis()->SetLimits(0,4);
    CMAP(n2D,"Stn 12 & 18;Track Momentum, p [GeV];Cluster Energy, E [GeV]",fname2D);

  return 0;
} 
