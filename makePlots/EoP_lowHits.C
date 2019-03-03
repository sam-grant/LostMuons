//Code for EoP for the low hit runs
// Sam Grant - Jan 2019
#include <iostream>
#include "TFile.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TPaveStats.h"
#include "TStyle.h"

using namespace std;

int main() {

  const int stn = 12; 

  string h1 = "St"+to_string(stn)+"_n_EoP_0";
  string h2 = "St"+to_string(stn)+"_mu_EoP_0";

  TFile *f1 = TFile::Open("../run_5/merged_Omega_lowHits.root");

  string fname = "../plots/talk5/EoP_"+to_string(stn)+"_bigBlock_lowHits.pdf";

   string title = "Stn "+to_string(stn)+": Runs with Low Lost Muon Populations;Track Momentum [MeV];Energy in Calorimeter [MeV]";
  // string title = "Stn "+to_string(stn)+": Low Hits | -2.2 < Log(E/p) < -1.6;Track Momentum [MeV];Energy in Calorimeter [MeV]";
  
  TH1D *n = (TH1D*)f1->Get(h1.c_str());
  TH1D *mu = (TH1D*)f1->Get(h2.c_str());
 
  // n -> Add(mu,1);
  
  f1->Save();

  TCanvas *c1 = new TCanvas();//"A3","Plotting Canvas",150,10,1100,660);
  gStyle->SetPalette(55);
  n->GetYaxis()->SetTitleOffset(1.5);
  n->SetTitle(title.c_str());
  n->SetStats(0);
  //  n->GetYaxis()->SetRangeUser(0,500);
  n->GetXaxis()->SetRangeUser(0,3500);
  n->Draw("COLZ");
  // Save
  c1 -> SaveAs(fname.c_str());

  delete n,mu,c1,f1;
  return 0;

}
