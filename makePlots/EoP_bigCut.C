//Code to overlay EoP mom plots for tracker & calo
// Sam Grant - March 2019

#include <iostream>
#include "TFile.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TPaveStats.h"
#include "TStyle.h"

using namespace std;

int main() {

  const int stn = 12; 

  string h1 = "St"+to_string(stn)+"_n_EoP";
  string h2 = "St"+to_string(stn)+"_mu_EoP";

  TFile *f1 = TFile::Open("../cluster_bigCut/merged_bigCut.root");

  string fname1 = "../plots/stn"+to_string(stn)+"_Eop_bigCut.png";
  string fname2 = "../plots/stn"+to_string(stn)+"_Eop_n_bigCut.png";
  string fname3 = "../plots/stn"+to_string(stn)+"_Eop_mu_bigCut.png";

  string title1 = "Stn "+to_string(stn)+" | All Tracker & Calo Tracks;Track Momentum [MeV];Energy in Calorimeter [MeV]";
  string title2 = "Stn "+to_string(stn)+" | Non Lost Muons (Tracker & Calo);Track Momentum [MeV];Energy in Calorimeter [MeV]";
  string title3 = "Stn "+to_string(stn)+" | Lost Muons (Tracker & Calo);Track Momentum [MeV];Energy in Calorimeter [MeV]";

  TH1D *n = (TH1D*)f1->Get(h1.c_str());
  TH1D *mu = (TH1D*)f1->Get(h2.c_str());
  TH1D *combine = (TH1D*)n->Clone("combine");
  combine->Add(mu,1);

  TCanvas *c1 = new TCanvas();
  gStyle->SetPalette(55);
  combine->GetYaxis()->SetTitleOffset(1.5);
  combine->SetTitle(title1.c_str());
  combine->SetStats(0);
   combine->GetYaxis()->SetRangeUser(0,4000);
  combine->GetXaxis()->SetRangeUser(0,3500);
  combine->Draw("COLZ");
  // mu->Draw("col sames");
  // Save
  c1 -> SaveAs(fname1.c_str());

  TCanvas *c2 = new TCanvas();
  gStyle->SetPalette(55);
  n->GetYaxis()->SetTitleOffset(1.5);
  n->SetTitle(title2.c_str());
  n->SetStats(0);
  n->GetYaxis()->SetRangeUser(0,4000);
  n->GetXaxis()->SetRangeUser(0,3500);
  n->Draw("COLZ");
  // mu->Draw("col sames");
  // Save
  c2 -> SaveAs(fname2.c_str());

  TCanvas *c3 = new TCanvas();
  gStyle->SetPalette(55);
  mu->GetYaxis()->SetTitleOffset(1.5);
  mu->SetTitle(title3.c_str());
  mu->SetStats(0);
  mu->GetYaxis()->SetRangeUser(90,300);
  mu->GetXaxis()->SetRangeUser(2200,3500);
  mu->Draw("COLZ");
  // mu->Draw("col sames");
  // Save
  c3 -> SaveAs(fname3.c_str());


  delete n, f1, c1, c2, c3, n, mu, combine;
  return 0;

}
