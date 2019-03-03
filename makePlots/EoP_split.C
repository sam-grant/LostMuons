//Code to generate EoP for muon region
// Sam Grant - Jan 2019
#include <iostream>
#include "TFile.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TPaveStats.h"
#include "TStyle.h"
#include "TLegend.h"

using namespace std;

int main() {

  const int stn = 18; 

  TFile *f1 = TFile::Open("../cluster_split/merged_split.root");

  string h1 = "St"+to_string(stn)+"_mu_EoP_0";
  string h2 = "St"+to_string(stn)+"_mu_EoP_1";

  string fname1 = "../plots/stn_"+to_string(stn)+"_EoP_upper.png";
  string fname2 = "../plots/stn_"+to_string(stn)+"_EoP_lower.png";
  string fname3 = "../plots/stn_"+to_string(stn)+"_EoP_lower_and_lower.png";


  string title1 = "Stn "+to_string(stn)+" | -2.4 < Log(E/p) < -1.6;Track Momentum [MeV];Energy in Calorimeter [MeV]";
  string title2 = "Stn "+to_string(stn)+" | -3.2 < Log(E/p) < -2.4;Track Momentum [MeV];Energy in Calorimeter [MeV]";
string title3 = "Stn "+to_string(stn)+" | -3.2 < Log(E/p) < -1.6;Track Momentum [MeV];Energy in Calorimeter [MeV]";


  TH1D *mu0 = (TH1D*)f1->Get(h1.c_str());
  TH1D *mu1 = (TH1D*)f1->Get(h2.c_str());
  TH1D *combine = (TH1D*)mu0->Clone("combine");
  combine->Add(mu1,1);

  f1->Save();

  TCanvas *c1 = new TCanvas();
  gStyle->SetPalette(55);
  mu0->GetYaxis()->SetTitleOffset(1.5);
  mu0->SetTitle(title1.c_str());
  mu0->SetStats(0);
  mu0->GetXaxis()->SetRangeUser(2200,3200);
  mu0->GetYaxis()->SetRangeUser(0,600);
  mu0->Draw("COLZ");
  c1 -> SaveAs(fname1.c_str());

  TCanvas *c2 = new TCanvas();
  gStyle->SetPalette(55);
  mu1->GetYaxis()->SetTitleOffset(1.5);
  mu1->SetTitle(title2.c_str());
  mu1->SetStats(0);
  mu1->GetXaxis()->SetRangeUser(2200,3200);
  mu1->GetYaxis()->SetRangeUser(0,600);
  mu1->Draw("COLZ");
  c2 -> SaveAs(fname2.c_str());

  TCanvas *c3 = new TCanvas();
  gStyle->SetPalette(55);
  combine->GetYaxis()->SetTitleOffset(1.5);
  combine->SetTitle(title3.c_str());
  combine->SetStats(0);
  combine->GetXaxis()->SetRangeUser(2200,3200);
  combine->GetYaxis()->SetRangeUser(0,600);
  combine->Draw("COLZ");
  c3 -> SaveAs(fname3.c_str());

  delete mu0, mu1, f1, c1, c2, c3, combine;
  return 0;

}
