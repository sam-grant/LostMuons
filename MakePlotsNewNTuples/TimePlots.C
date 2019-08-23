// Code to create time plots for lost muons
// Sam Grant - March 2019

#include <iostream>
#include "TFile.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TPaveStats.h"
//#include "THStack.h"
#include "TLegend.h"
//#include "TStyle.h"

using namespace std;

int main() {

  const int stn = 18;

  string title1 = "Stn "+to_string(stn)+": Time Distrubtion for Lost Muons;Absolute Time [ns];Number of Hits";

  string fname1 = "../plots/stn"+to_string(stn)+"_mu_time.pdf";
 
  string h1 = "St"+to_string(stn)+"_mu_time";

  TFile *f1 = TFile::Open("../cluster_master/merged_cluster.root");
 
  TH1D *mu = (TH1D*)f1->Get(h1.c_str());

  mu -> SetName("Lost Muons (Tracker & Calo)");

  // Save File  
  f1->Save();
 
  // Colour in
  mu->SetLineColor(kRed+2);
  mu->SetMarkerColor(kRed+2);

  // Draw and Save
  TCanvas *c1 = new TCanvas();
  mu->SetTitle(title1.c_str());
  mu->GetYaxis()->SetMaxDigits(3);
  mu->Draw();
  c1->SetLogy();
  c1->SaveAs(fname1.c_str());
 
  //  delete f1, mu, c1;
  return 0;

}
