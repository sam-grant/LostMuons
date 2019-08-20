//Code to overlay EoP mom plots for tracker and cluster to see if they line up at 2 GeV 
// Sam Grant - Jan 2019
#include <iostream>
#include "TFile.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TPaveStats.h"
#include "TStyle.h"

using namespace std;

int main() {

  const int stn = 18
; 

  string h1 = "St"+to_string(stn)+"_n_EoP_0";
  string h2 = "St"+to_string(stn)+"_n_EoP_1";
  string h3 = "St"+to_string(stn)+"_mu_EoP_0";
  string h4 = "St"+to_string(stn)+"_mu_EoP_1";

  TFile *f1 = TFile::Open("../run_5/merged_Omega_bigBlock_tcut.root");

  string fname1 = "../plots/talk5/stn"+to_string(stn)+"_EoP_bigBlock_n_tcut_0.pdf";
  string fname2 = "../plots/talk5/stn"+to_string(stn)+"_EoP_bigBlock_n_tcut_1.pdf";

  string title1 = "Stn "+to_string(stn)+": t < 30,000 ns;Track Momentum [MeV];Energy in Calorimeter [MeV]";
  string title2 = "Stn "+to_string(stn)+": t > 30,000 ns;Track Momentum [MeV];Energy in Calorimeter [MeV]";

  TH1D *n0 = (TH1D*)f1->Get(h1.c_str());
  TH1D *n1 = (TH1D*)f1->Get(h2.c_str());
  TH1D *mu0 = (TH1D*)f1->Get(h3.c_str());
  TH1D *mu1 = (TH1D*)f1->Get(h4.c_str());

  n0 -> Add(mu0,1);
  n1 -> Add(mu1,1);

  f1->Save();

  TCanvas *c1 = new TCanvas();//"A3","Plotting Canvas",150,10,1100,660);
  gStyle->SetPalette(55);
  n0->GetYaxis()->SetTitleOffset(1.5);
  n0->SetTitle(title1.c_str());
  n0->SetStats(0);
  n0->GetYaxis()->SetRangeUser(0,4000);
  n0->GetXaxis()->SetRangeUser(0,3500);
  n0->Draw("COLZ");
  // Save
  c1 -> SaveAs(fname1.c_str());

  TCanvas *c2 = new TCanvas();//"A3","Plotting Canvas",150,10,1100,660);
  gStyle->SetPalette(55);
  n1->GetYaxis()->SetTitleOffset(1.5);
  n1->SetTitle(title2.c_str());
  n1->SetStats(0);
  n1->GetYaxis()->SetRangeUser(0,4000);
  n1->GetXaxis()->SetRangeUser(0,3500);
  n1->Draw("COLZ");
  // Save
  c2 -> SaveAs(fname2.c_str());

  delete n0, n1, f1, c1, c2;
  return 0;

}
