//Code to make run number plots
// Sam Grant - Jan 2019
#include <iostream>
#include "TFile.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TStyle.h"
//#include "TAxis.h"

using namespace std;

int main() {
  
  int stn = 12;

  string h1 = "St"+to_string(stn)+"_n_mom";
  string h2 = "St"+to_string(stn)+"_mu_mom";
  string h3 = "St"+to_string(stn)+"_n_energy";
  string h4 = "St"+to_string(stn)+"_mu_energy";

  string fname1 = "../plots/stn"+to_string(stn)+"_mom_shoulder2.pdf";
  string fname2 = "../plots/stn"+to_string(stn)+"_energy_shoulder2.pdf";

  string title1 = "Stn "+to_string(stn)+" | Momentum Distributions;Track Momentum [MeV];Number of Hits";
  string title2 = "Stn "+to_string(stn)+" | Energy Distributions;Energy in Calo [MeV];Number of Hits";

  TFile *f1 = TFile::Open("../cluster_shoulder/merged_shoulder2.root");

  TH1D *np = (TH1D*)f1->Get(h1.c_str());
  TH1D *mup = (TH1D*)f1->Get(h2.c_str());
  TH1D *nE = (TH1D*)f1->Get(h3.c_str());
  TH1D *muE = (TH1D*)f1->Get(h4.c_str());

  np -> SetName("Non Lost Muons (Tracker & Calo)");
  mup -> SetName("Lost Muons (Tracker & Calo)");
  nE -> SetName("Non Lost Muons (Tracker & Calo)");
  muE -> SetName("Lost Muons (Tracker & Calo)");

  f1->Save();
 
  TCanvas *c1 = new TCanvas();
  np->Draw();
  mup->Draw("same");
  c1->BuildLegend(0.70,0.80,0.89,0.89);
  np->SetStats(0);
  mup->SetStats(0);
  np->GetYaxis()->SetMaxDigits(3);
  np->SetLineColor(kRed+2);
  np->SetMarkerColor(kRed+2);
  mup->SetLineColor(kBlue+2);
  mup->SetMarkerColor(kBlue+2);
  np->SetTitle(title1.c_str());

  // Save
  c1 -> SaveAs(fname1.c_str());

  TCanvas *c2 = new TCanvas();
  nE->Draw();
  muE->Draw("same");
  c2->BuildLegend(0.70,0.80,0.89,0.89);
  nE->SetStats(0);
  muE->SetStats(0);
  nE->GetYaxis()->SetMaxDigits(3);
  nE->GetXaxis()->SetRangeUser(0,3500);
  nE->SetLineColor(kRed+2);
  nE->SetMarkerColor(kRed+2);
  muE->SetLineColor(kBlue+2);
  muE->SetMarkerColor(kBlue+2);
  nE->SetTitle(title2.c_str());

  // Save
  c2 -> SaveAs(fname2.c_str());


  delete c1, c2, f1, np, nE, mup, muE;
  return 0;
} 
