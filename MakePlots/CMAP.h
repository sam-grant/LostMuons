// cmap
// ---------------
// Function to make colourise 2D histograms, make them look nice, and set sensible ranges. It will also wrtie the results to ROOT file and/or .png

// Arguments: input 2D histogram, output histogram title, output histogram filename, name of output ROOT file, and an option to save output to .png (always gets written to ROOT file). 

// - Sam Grant, April 2019 
// - samuel.grant.18@ucl.ac.uk

#ifndef CMAP_h
#define CMAP_h

#include <iostream>
#include "TH2D.h"
#include "TFile.h"
#include "TStyle.h"
#include "TDirectory.h"
#include "TCanvas.h"

using namespace std;

//void CMAP(TH2D *hist, string title, string fname, TFile *output, bool save) {
void CMAP(TH2D *hist, string title, string fname) {
  //Clone input to be safe 
  TH2D *hist_clone = (TH2D*)hist->Clone("hist_clone");
  //Force the ranges to be sensible if ROOT's stupid autoscale fails like normal
  double xmin = hist_clone->GetXaxis()->GetBinCenter( hist_clone -> FindFirstBinAbove(0,1) );
  double xmax = hist_clone->GetXaxis()->GetBinCenter( hist_clone -> FindLastBinAbove(0,1) );
   hist_clone->GetXaxis()->SetRangeUser(xmin,xmax);
  double ymin = hist_clone->GetYaxis()->GetBinCenter( hist_clone -> FindFirstBinAbove(0,2) );
  double ymax = hist_clone->GetYaxis()->GetBinCenter( hist_clone -> FindLastBinAbove(0,2) );
  hist_clone->GetYaxis()->SetRangeUser(ymin,ymax);
  // Set name...
  hist_clone->SetName(fname.c_str());
  // Draw it
  TCanvas *c = new TCanvas("c", "c", 3000, 2000);
   hist_clone->SetStats(0);
  hist_clone->SetTitle(title.c_str());
  c->SetRightMargin(0.13);
  gStyle->SetPalette(55); 
  hist_clone->SetOption("COLZ");
  hist_clone->Draw();
  // if (save) {
  c->SaveAs(fname.c_str());//("Plots/"+fname).c_str()); 
    // }
  //  hist_clone->SetDirectory(output);
  delete c;
  return;
}

#endif
