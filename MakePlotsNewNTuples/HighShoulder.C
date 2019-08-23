// Marco to draw momentum distributions
// Sam Grant - Jan 2019

#include <iostream>
#include "TFile.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TLegend.h"
#include "CMAP.h"

using namespace std;

void draw(TH1D *h1, TH1D *h2, string title, string fname) {

  gStyle->SetLegendTextSize(0.03);

  TCanvas *c = new TCanvas("c","c",3000,2000);
  TLegend *leg = new TLegend(0.62,0.85,0.95,0.95);

  //  leg->SetBorderSize(0);
  leg->AddEntry(h1,"Non lost muons (track-cluster)");
  leg->AddEntry(h2,"Lost muons (track-cluster)");
  h1->SetStats(0);
  h2->SetStats(0);
  h1->SetLineColor(kRed);
  h2->SetLineColor(kGreen-3);
  h1->SetLineWidth(10);
  h2->SetLineWidth(10);
  h1->SetTitle(title.c_str());
  h1->Draw();
  h2->Draw("same");
  leg->Draw("same");
  c->SaveAs(fname.c_str());

  delete c;
  return;
  
}

int main() {

  string fname[2] = {"Plots/MomentumNoHighEnergyCut.png","Plots/MomentumHighEnergyCut.png"};
  string fname2D[2] = {"Plots/EvpNoHighEnergyCut.png","Plots/EvpHighEnergyCut.png"};
  /* string fname[2] = {"Plots/MomentumNoHighEnergyCutComplete.png","Plots/MomentumHighEnergyCutComplete.png"}; */
  /* string fname2D[2] = {"Plots/EvpNoHighEnergyCutComplete.png","Plots/EvpHighEnergyCutComplete.png"}; */
  string inputName[2] = {"TrackCluster","TrackClusterHighEnergyCut"};

  // TH2D *hist, string title, string fname, TFile *output, bool save
  for(int icut(0); icut<2; icut++) {

    TFile *input = TFile::Open(("../ReadOldNTuples/"+inputName[icut]+"/RootFiles/"+inputName[icut]+".root").c_str());

    TH1D *n = (TH1D*)input->Get("p_1"); 
    TH1D *mu = (TH1D*)input->Get("p_2");
    n->GetXaxis()->SetLimits(0,3.5);
    draw(n,mu,"Stn 12 & 18;Track Momentum, p [GeV];Entries",fname[icut]);

    TH2D *n2D = (TH2D*)input->Get("Evp_1");
    n2D->GetXaxis()->SetLimits(0,4);//;
    n2D->GetYaxis()->SetLimits(0,4);
    CMAP(n2D,"Stn 12 & 18;Track Momentum, p [GeV];Cluster Energy, E [GeV]",fname2D[icut]);
  }  

  return 0;
} 
