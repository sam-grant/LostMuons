//Code to make mom tcut plots
// Sam Grant - Feb 2019
#include <iostream>
#include "TFile.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TPaveStats.h"

using namespace std;

int main() {
  
  int stn = 18;

  string h1 = "St"+to_string(stn)+"_n_mom_0";
  string h2 = "St"+to_string(stn)+"_n_mom_1";

  string fname1 = "../plots/talk5/stn"+to_string(stn)+"_bigBlock_n_mom_tcut.pdf";

  //string fname3 = "../plots/talk5/stn"+to_string(stn)+"_run_v_p_all.pdf";

  string title1 = "Stn "+to_string(stn)+": Non Lost Muons | E > 1000 MeV & p < 2300 MeV;Track Momentum [MeV];Number of Hits (Scaled)";
  // string title1 = "Stn "+to_string(stn)+": Non Lost Muons | E > 1000 MeV & p < 2300 MeV;Track Momentum [MeV];Number of Hits";

  TFile *f1 = TFile::Open("../run_5/merged_Omega_bigBlock_tcut.root");

  TH1D *n0 = (TH1D*)f1->Get(h1.c_str());
  TH1D *n1 = (TH1D*)f1->Get(h2.c_str());
  //TH1D *muAll = (TH1D*)f2->Get(h1.c_str());
  n0->SetName("t < 30,000 ns");
  n1->SetName("t > 30,000 ns");

  // Save 
  f1->Save();

  n0->Draw();
  gPad->Update();

  TPaveStats *tps1 = (TPaveStats*) n0 -> FindObject("stats"); 

  tps1 -> SetTextColor(kRed+2);
  tps1 -> SetLineColor(kRed+2);

  double X1 = tps1->GetX1NDC();
  double Y1 = tps1->GetY1NDC();
  double X2 = tps1->GetX2NDC();
  double Y2 = tps1->GetY2NDC();

  n1->Draw();
  gPad->Update();

  TPaveStats *tps2 = (TPaveStats*) n1-> FindObject("stats");
   
  tps2->SetTextColor(kBlue+2);
  tps2->SetLineColor(kBlue+2);

  tps2->SetX1NDC(X1);
  tps2->SetX2NDC(X2); 
  tps2->SetY1NDC(Y1-(Y2-Y1)); 
  tps2->SetY2NDC(Y1);

  // Relative normalisation
  double sf =  n0 -> Integral() / n1 -> Integral() ;
  n1 -> Scale( sf ); 
  cout << "Scale Factor = " << sf << endl;

  TCanvas *c1 = new TCanvas("c1","c1",150,10,1100,660);
  //n0->GetYaxis()->SetMaxDigits(2);
  n0->SetLineColor(kRed+2);
  n0->SetMarkerColor(kRed+2);
  n1->SetLineColor(kBlue+2);
  n1->SetMarkerColor(kBlue+2);
  n0->GetYaxis()->SetTitleOffset(1.5);
  n0->GetXaxis()->SetRangeUser(0,3500);
  n0->SetTitle(title1.c_str());
  n0->Draw();
  n1->Draw("same");
  tps1->Draw("same");
  tps2->Draw("same");
  // Save
  c1 -> SaveAs(fname1.c_str());

  delete c1, f1, n0, n1;
  return 0;
} 
