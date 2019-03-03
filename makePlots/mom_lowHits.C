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
  
  int stn = 18;

  string h1 = "St"+to_string(stn)+"_n_mom_0";
  //  string h2 = "St"+to_string(stn)+"_n_p_1";

  string fname1 = "../plots/talk5/stn"+to_string(stn)+"_bigBlock_n_mom_lowHits.pdf";
  // string fname2 = "../plots/talk5/stn"+to_string(stn)+"_run_v_p_1.pdf";
  //string fname3 = "../plots/talk5/stn"+to_string(stn)+"_run_v_p_all.pdf";

  string title1 = "Stn "+to_string(stn)+": Non Lost Muons | E > 1000 MeV & p < 2300 MeV;Track Momentum [MeV];Number of Hits";


  TFile *f1 = TFile::Open("../run_5/merged_Omega_lowHits.root");

  TH1D *n = (TH1D*)f1->Get(h1.c_str());
  // TH1D *mu1 = (TH1D*)f1->Get(h2.c_str());
  //TH1D *muAll = (TH1D*)f2->Get(h1.c_str());

  // Save 
  f1->Save();
  //f2->Save();
  //gStyle->SetPalette(55);
  TCanvas *c1 = new TCanvas("c1","Plotting Canvas",150,10,1100,660);

  // mu0->GetYaxis()->SetRangeUser(2300,4000);
  //mu0->SetStats(0);
  // mu0->GetYaxis()->SetTitleOffset(1.5);
  n->SetLineColor(kRed+2);
    n->SetMarkerColor(kRed+2);
  n->SetTitle(title1.c_str());
  n->Draw();
  // Save
  c1 -> SaveAs(fname1.c_str());



  delete c1, f1, n;
  return 0;
} 
