//Code to make tracker radii plots and subtracted radii plots
// Sam Grant - March 2019
#include <iostream>
#include "TFile.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TPaveStats.h"
#include "TStyle.h"
#include "TLegend.h"

using namespace std;

int main() {

  int stn = 12;

  // Apply scaling? 
  // s = 0 scale=true ; s = 1 scale=false
  int s = 1; 

  double sfMu;
  // double sfMu_err;
  double sfN;
  // double sfN_err;

  // Hard code scale factors
  if ( stn == 12 ) {
    sfMu = 1.43700046271415;
    // sfMu_err = 0.026832843957049;
    sfN = 1.05470880017382;
    // sfN_err = 0.001669350170777;
  }
  else if ( stn == 18 ) {
    sfMu = 1.59170023827753;
    // sfMu_err = 0.023022762229112;
    sfN = 1.05737306236386;
    //sfN_err = 0.001640351508775;
  }
  

  string h1 = "St"+to_string(stn)+"_radius";
  string h2 = "St"+to_string(stn)+"_mu_radius";
  string h3 = "St"+to_string(stn)+"_n_radius";
  string fname1;
  string fname2 = "../plots/stn"+to_string(stn)+"_tracker_radius_check.pdf";
  if (s == 0 ) {
    fname1 = "../plots/stn"+to_string(stn)+"_tracker_radius_subtracted.pdf";
  }
  else if ( s == 1 ) { 
    fname1 = "../plots/stn"+to_string(stn)+"_tracker_radius_subtracted_noscale.pdf";
  }

  string title1 = "Stn "+to_string(stn)+" | Radial Decay Vertices;Radial Decay Vertex [mm];Number of Hits";
  string title2 = "Stn "+to_string(stn)+" | Sanity Check";

  TFile *f1 = TFile::Open("../tracker/merged_tracker.root");
  TFile *f2 = TFile::Open("../cluster_master/merged_cluster.root");
 
  TH1D *tr = (TH1D*)f1->Get(h1.c_str());
  TH1D *mu = (TH1D*)f2->Get(h2.c_str());
  TH1D *n = (TH1D*)f2->Get(h3.c_str());

  // Scale
  if ( s == 0 ) {
    mu -> Scale(sfMu);
    n -> Scale(sfN);
  }
 
  // Subtract
  TH1D *subMu = (TH1D*)tr->Clone("subMu");
  TH1D *subN = (TH1D*)tr->Clone("subN");

  subMu -> Add(mu,-1);
  subN -> Add(n, -1);

  //  delete n, mu; 

  // Rename for presentation

  tr->SetName("All Tracks");
  if ( s == 0 ) {
    subMu->SetName("All Tracks Minus Lost Muons (Scaled)");

    subN->SetName("All Tracks Minus Non Lost Muons (Scaled)");
  }

  else if ( s == 1 ) {
    subMu->SetName("All Tracks Minus Lost Muons (Unscaled)");

    subN->SetName("All Tracks Minus Non Lost Muons (Unscaled)");
  }

  f1->Save();
  f2->Save();
   
  // Draw first histogram
  tr -> Draw();
  gStyle->SetOptStat(2211);
  gPad->Update();

  // Collect stats of the first histogram 

  TPaveStats *tps1 = (TPaveStats*) tr -> FindObject("stats"); 

  tps1->SetTextColor(kBlack);
  tps1->SetLineColor(kBlack);
  
  tps1->SetX1NDC(0.75);
  tps1->SetX2NDC(0.99);
  tps1->SetY1NDC(0.85);
  tps1->SetY2NDC(0.99);
  
  double X1 = tps1->GetX1NDC();
  double Y1 = tps1->GetY1NDC();
  double X2 = tps1->GetX2NDC();
  double Y2 = tps1->GetY2NDC();

  // Draw second histogram
  subMu -> Draw();
  gStyle->SetOptStat(2211);
  gPad->Update();

  // Collect stats of the second histogram
     
  TPaveStats *tps2 = (TPaveStats*) subMu -> FindObject("stats");
  
  tps2->SetTextColor(kRed+2);
  tps2->SetLineColor(kRed+2);

  // Put stat box beneath the first
  tps2->SetX1NDC(X1);
  tps2->SetX2NDC(X2); 
  tps2->SetY1NDC(Y1-(Y2-Y1)); 
  tps2->SetY2NDC(Y1);

  // Draw third histogram
  subN -> Draw();
  gStyle->SetOptStat(2211);
  gPad->Update();

  // Collect stats of the third histogram

  TPaveStats *tps3 = (TPaveStats*) subN -> FindObject("stats");
  
  tps3->SetTextColor(kBlue+2);
  tps3->SetLineColor(kBlue+2);

  // Put stat box beneath the first
  tps3->SetX1NDC(X1);
  tps3->SetX2NDC(X2); 
  tps3->SetY1NDC(Y1-2*(Y2-Y1)); 
  tps3->SetY2NDC(Y1-(Y2-Y1));
  
  // Draw histograms on one canvas

  TCanvas *c1 = new TCanvas();
  // gStyle->SetOptStat(2211);
  tr->SetMarkerColor(kBlack);
  tr->SetLineColor(kBlack);
  subMu->SetMarkerColor(kRed+2);
  subMu->SetLineColor(kRed+2);
  subN->SetMarkerColor(kBlue+2);
  subN->SetLineColor(kBlue+2);
  // tr->GetYaxis()->SetRangeUser(0,1E7);
  tr->GetXaxis()->SetRangeUser(-250,250);
  tr->SetTitle(title1.c_str());
  // c1->SetLogy();
  tr->Draw();
  subMu->Draw("same");
  subN->Draw("same");
  tps1->Draw("same"); 
  tps2->Draw("same");
  tps3->Draw("same");  
  c1 -> SaveAs(fname1.c_str());

  /*

  // Sanity Check 
  TH1D *combine = (TH1D*)tr -> Clone("combine");
  combine -> Add(mu,1);
  combine -> Add(n,1);
  combine -> SetName("Lost Muons (Scaled) + Non Lost Muons (Scaled)");
  
  TCanvas *c2 = new TCanvas();
  tr->SetStats(0);
  combine->SetStats(0);
  tr->SetMarkerColor(kGreen+2);
  tr->SetLineColor(kGreen+2);
  combine->SetMarkerColor(kMagenta+2);
  combine->SetLineColor(kMagenta+2);
  tr->Draw();
  combine->Draw("same");
  c2->BuildLegend(0.69,0.80,0.99,0.99);
  tr->GetXaxis()->SetRangeUser(-250,250);
  tr->SetTitle(title2.c_str());
  c2 -> SaveAs(fname2.c_str());
  

  */
    
  delete c1, c2, tr, subN, subMu, combine;
  return 0;
} 
