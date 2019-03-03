// Code for logEoP plots 
// Sam Grant - Feb 2019

#include <iostream>
#include "TFile.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TPaveStats.h"

using namespace std;

int main() {

  const int stn = 18; 
 
  string h1 = "St"+to_string(stn)+"_n_logEoP_0";
  string h2 = "St"+to_string(stn)+"_mu_logEoP_0";

  string fname = "../plots/LogEoP/timeCut/stn"+to_string(stn)+"_LogEoP.pdf";

  TFile *f1 = TFile::Open("../run_cl_tcut/merged_Omega_cluster.root");

  TH1D *n = (TH1D*)f1->Get(h1.c_str());
  TH1D *mu = (TH1D*)f1->Get(h2.c_str());

  n->SetName("Non Lost Muons");
  mu->SetName("Lost Muons");

  f1->Save();

  n->Draw();
  gPad->Update();
 
  TPaveStats *tps1 = (TPaveStats*) n->FindObject("stats"); 

  double X1 = tps1->GetX1NDC();
  double Y1 = tps1->GetY1NDC();
  double X2 = tps1->GetX2NDC();
  double Y2 = tps1->GetY2NDC();

  mu->Draw();
  gPad->Update();

 TPaveStats *tps2 = (TPaveStats*) mu->FindObject("stats");;

 tps2->SetX1NDC(X1);
 tps2->SetX2NDC(X2); 
 tps2->SetY1NDC(Y1-(Y2-Y1)); 
 tps2->SetY2NDC(Y1);

 TCanvas *c1 = new TCanvas();

 n->SetMarkerColor(kRed+2);
 n->SetLineColor(kRed+2);
 mu->SetMarkerColor(kBlue+2);
 mu->SetLineColor(kBlue+2);
 tps1->SetTextColor(kRed+2);
 tps1->SetLineColor(kRed+2);
 tps2->SetTextColor(kBlue+2);
 tps2->SetLineColor(kBlue+2);

  n->GetXaxis()->SetRangeUser(-6,6);

  string title = "Stn "+to_string(stn)+": Log Energy over Momentum;Log(E/p);Number of Hits";
  n->SetTitle(title.c_str());
  n->Draw();
  mu->Draw("same");
  tps1->Draw("same"); 
  tps2->Draw("same");

  c1->SetLogy();

  c1->SaveAs(fname.c_str());
 
  delete mu, n, tps1, tps2, c1;
  return 0;

}
