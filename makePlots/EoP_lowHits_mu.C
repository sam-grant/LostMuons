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

  string h1 = "St"+to_string(stn)+"_mu_EoP_1";

  TFile *f1 = TFile::Open("../run_5/merged_Omega_lowHits.root");

  string fname = "../plots/talk5/EoP_mu_"+to_string(stn)+"_lowHits_1.pdf";

  // string title = "Stn "+to_string(stn)+": Low Hits | -3.5 < Log(E/p) < -2.2;Track Momentum [MeV];Energy in Calorimeter [MeV]";
  string title = "Stn "+to_string(stn)+": Low Hits | -2.2 < Log(E/p) < -1.6;Track Momentum [MeV];Energy in Calorimeter [MeV]";
  TH1D *mu = (TH1D*)f1->Get(h1.c_str());

  f1->Save();

  TCanvas *c1 = new TCanvas();//"A3","Plotting Canvas",150,10,1100,660);
  gStyle->SetPalette(55);
  mu->GetYaxis()->SetTitleOffset(1.5);
  mu->SetTitle(title.c_str());
  mu->SetStats(0);
  mu->GetYaxis()->SetRangeUser(0,500);
  mu->GetXaxis()->SetRangeUser(2300,3100);
  mu->Draw("COLZ");
  // Save
  c1 -> SaveAs(fname.c_str());

  delete mu,c1,f1;
  return 0;

}
