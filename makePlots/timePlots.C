// Code to create time plots for lost muons
// Sam Grant - Feb 2019
#include <iostream>
#include "TFile.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TPaveStats.h"
//#include "TFractionFitter.h"
//#include "THStack.h"
//#include "TLegend.h"
//#include "TStyle.h"

using namespace std;

int main() {

  const int stn = 12;

  string title1 = "Stn "+to_string(stn)+": Time Distrubtion for Lost Muons | E < 1000 MeV & p > 2300 MeV;Absolute Time [ns];Number of Hits";
  string fname1 = "../plots/talk5/stn"+to_string(stn)+"_time_tcut_newData.pdf";

  string h1 = "St"+to_string(stn)+"_mu_time_0";
  string h2 = "St"+to_string(stn)+"_mu_time_1";

  TFile *f1 = TFile::Open("../run_5_newData/merged_Omega_bigBlock_tcut.root");
 
  TH1D *mu0 = (TH1D*)f1->Get(h1.c_str());
  TH1D *mu1 = (TH1D*)f1->Get(h2.c_str());

  mu0->SetName("t < 30,000 ns");
  mu1->SetName("t > 30,000 ns");

  // Save File  
  f1->Save();

  mu0->Draw();
  gPad->Update();

  TPaveStats *tps1 = (TPaveStats*) mu0->FindObject("stats");

  double X1 = tps1->GetX1NDC();
  double Y1 = tps1->GetY1NDC();
  double X2 = tps1->GetX2NDC();
  double Y2 = tps1->GetY2NDC();

  mu1->Draw();
  gPad->Update();

  TPaveStats *tps2 = (TPaveStats*) mu1->FindObject("stats");;

  tps2->SetX1NDC(X1);
  tps2->SetX2NDC(X2);
  tps2->SetY1NDC(Y1-(Y2-Y1));
  tps2->SetY2NDC(Y1);
  
  // Colour in
  mu0->SetLineColor(kRed+2);
  mu0->SetMarkerColor(kRed+2);
  tps1->SetLineColor(kRed+2);
  tps1->SetTextColor(kRed+2);

  mu1->SetLineColor(kBlue+2);
  mu1->SetMarkerColor(kBlue+2);
  tps2->SetLineColor(kBlue+2);
  tps2->SetTextColor(kBlue+2);

  // Draw and Save
  TCanvas *c1 = new TCanvas();
  mu0->SetTitle(title1.c_str());
  mu0->GetYaxis()->SetMaxDigits(3);
  mu0->Draw();
  mu1->Draw("same");
  tps1->Draw("same");
  tps2->Draw("same");
  
  c1->SaveAs(fname1.c_str());
 
  delete f1, mu0, mu1, tps1, tps2, c1;
  return 0;

}
