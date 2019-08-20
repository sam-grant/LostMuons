
//Code to find the fraction of fiducial and non fiducial events between tracker and cluster between 2.2 and 3.2 GeV 
// Sam Grant - Feb 2019

#include <iostream>
#include "TFile.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TPaveStats.h"
#include "TFractionFitter.h"
#include "THStack.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TObject.h"

using namespace std; 

int main() {

  const int stn = 12;

  // Fiducial volume = 0 ; Non Fiducial Volume = 1
  const int fid = 0;
  string fiducial;

  if (fid == 0 ) { fiducial = "Fiducial"; }
  if (fid == 1 ) { fiducial = "Non Fiducial"; }
  
  string h1 = "St"+to_string(stn)+"_mom_"+to_string(fid);
  string h2 = "St"+to_string(stn)+"_n_mom_dtdR_"+to_string(fid);
  string h3 = "St"+to_string(stn)+"_mu_mom_dtdR_"+to_string(fid);
 
  
  string fname1 = "../plots/fracFit_fid__"+to_string(stn)+"_dtdR_"+to_string(fid)+".pdf";
  string fname2 = "../plots/momRatio_fid_"+to_string(stn)+"_dtdR_"+to_string(fid)+".pdf";

  TFile *f1 = TFile::Open("../tracker_fiducial/merged_fiducial_tracker.root");
  TFile *f2 = TFile::Open("../cluster_fiducial/merged_fiducial_cluster.root");
  /*
  TFile *f1 = TFile::Open("../../run_tr/merged_Omega_tracker.root");
  TFile *f2 = TFile::Open("../../run_cl/merged_Omega_cluster.root");
  */
  string title1 = "Stn "+to_string(stn)+" | Fractional Fit ("+fiducial+"): 2300 MeV - 3000 MeV; Momentum [MeV]; Number of Hits";
  string title2 = "Stn "+to_string(stn)+" | Fractional Fit Region ("+fiducial+");Momentum [MeV];Number of Hits";

  TH1D *tr = (TH1D*)f1->Get(h1.c_str());
  TH1D *n = (TH1D*)f2->Get(h2.c_str());
  TH1D *mu = (TH1D*)f2->Get(h3.c_str());

  tr->SetName("All Tracks (Tracker)");
  n->SetName("Non Lost Muons (Tracker & Calo)");
  mu->SetName("Lost Muons (Tracker & Calo)");  

  f1->Save();
  f2->Save(); 
                                                  
  TObjArray *cl = new TObjArray(3); // cluster histograms are put in this array 
  n->SetLineColor(kRed+2);
  n->SetMarkerColor(kRed+2);
  mu->SetLineColor(kGreen+2);
  mu->SetMarkerColor(kGreen+2);
  tr->SetMarkerColor(kBlue+2);
  tr->SetLineColor(kBlue+2);
  
  cl->Add(mu);
  cl->Add(n);

  TFractionFitter* fit = new TFractionFitter(tr, cl); // initialise
  fit->SetRangeX(tr->GetXaxis()->FindBin(2300),tr->GetXaxis()->FindBin(3000));  

  Int_t status = fit->Fit(); //Perform the fit
  Double_t v0, v1, e0, e1; // Define fit parameters
  cout << "fit status: " << status << endl; // check on fit status

   if (status == 0) {                      
     TH1D* result = (TH1D*) fit->GetPlot(); // Book result
     fit->GetResult(0,v0,e0);
     fit->GetResult(1,v1,e1);

     TCanvas *c1 = new TCanvas();
     tr->SetStats(0);
     tr->GetYaxis()->SetMaxDigits(3);
     tr->GetXaxis()->SetRangeUser(0,3500);
     tr->Draw();
     cl->Draw("same");
     result->Draw("same");
     result->SetLineColor(kMagenta+2);
     c1->BuildLegend(0.65,0.75,0.89,0.89);
     // c1->BuildLegend(0.7, 0.65, 0.95, 0.84);
     tr->SetTitle(title1.c_str());
     // Save 

     c1 -> SaveAs(fname1.c_str()); // Visualise the fit
     delete c1;
   }

  delete cl;

   // Find calo efficiency

  int bin2300 = tr -> GetXaxis() -> FindBin(2300);
  int bin3000 = tr -> GetXaxis() -> FindBin(3000);

  double I_tot = tr -> Integral(bin2300, bin3000);
  double I_mu = mu -> Integral(bin2300, bin3000);
  double I_n = n -> Integral(bin2300, bin3000);

  double eff_mu = I_mu / (I_tot  * v0 ); 
  double eff_mu_err = eff_mu * (e0/v0);
  double eff_n = I_n / (I_tot * v1);
  double eff_n_err = eff_n * (e1/v1);



  // Cross check
  // n->Scale(1/eff_n);
  // mu->Scale(1/eff_mu);

  cout << "##################################################" << endl;
  cout << "Stn " << stn << " " << fiducial<< endl;
  cout << "Lost Muon Fraction = "<<v0<<" +/- "<<e0<<endl;
  cout << "Non Lost Muon Fraction = "<<v1<<" +/- "<<e1<<endl;
  cout<< "Lost Muon Efficiency  = "<<eff_mu*100<<" +/- "<<eff_mu_err*100<<"%"<<endl;
  cout<< "Non Lost Muons Efficiency  = "<<eff_n*100<<" +/- "<<eff_n_err*100<<"%"<<endl;
  cout << "Scale Factor Lost Muons = " << 1./eff_mu << endl;
  cout << "Scale Factor Non Lost Muons = " << 1./eff_n << endl;
  cout << "##################################################" << endl;

  // Add those suckers, do a cross check to make sure the cluster and tracker distributions line up at high p

  
  // Draw momentum distribution and ratios
  
  TH1D *ratio1 = (TH1D*)n->Clone("ratio1");
  TH1D *ratio2 = (TH1D*)mu->Clone("ratio2");
  ratio1->Divide(tr);
  ratio2->Divide(tr);

  TCanvas *c2 = new TCanvas();
 
  c2->Divide(1,2);
  c2->cd(1);
  tr->Draw();
  n->Draw("same");
  mu->Draw("same");
  tr->SetTitle(0);
  tr->SetName("All Tracks (Tracker)");
  //c2->cd(1)->BuildLegend(0.65,0.72,0.99,0.99);
  c2->cd(1)->BuildLegend(0.75,0.75,0.99,0.99);
  //tr->SetName("All Tracks (Tracker)");
  tr->SetStats(0);
  n->SetStats(0);
  mu->SetStats(0);
  tr->GetYaxis()->SetTitleOffset(0.75);
  //r->SetTitle(title2.c_str());
  tr->GetXaxis()->SetRangeUser(2300,3000);
  
  // tps1->Draw("same");
  // tps2->Draw("same");
  //tps3->Draw("same");
  tr->SetTitle(title2.c_str());

  c2->cd(2);

  ratio1->SetName("All Tracks / Non Lost Muons");
  ratio1->GetYaxis()->SetTitleOffset(0.75);
  ratio1->GetYaxis()->SetTitle("Fraction");
  ratio1->SetStats(0);
  ratio1->SetLineColor(kMagenta+2);
  ratio1->SetMarkerColor(kMagenta+2);
  ratio1->GetXaxis()->SetRangeUser(2300,3000);
  ratio1->GetYaxis()->SetRangeUser(0,1);
  // ratio1->SetTitle("Ratio;Momentum [MeV];Number of Hits") ;
  ratio1->Draw();
  // c2->cd(3);

  ratio2->SetName("All Tracks / Lost Muons");
  ratio2->SetStats(0);
  ratio2->SetLineColor(kOrange+2);
  ratio2->SetMarkerColor(kOrange+2);
  // c2->cd(2)->BuildLegend(0.65,0.75,0.89,0.89);
  // ratio2->SetTitle("Ratio;Momentum [MeV];Number of Hits") ;
  ratio2->Draw("same");


  /* TH1D *combined = (TH1D*)ratio1->Clone("combined");
  combined->Add(ratio2,+1);
  combined->SetStats(0);
  combined->SetName("Combined");
  combined->SetMarkerColor(kCyan+2);
  combined->SetLineColor(kCyan+2);
  combined->Draw("same");*/

  gPad->SetGrid();
  c2->cd(2)->BuildLegend(0.765,0.82,0.99,0.99);
  c2->SaveAs(fname2.c_str());

  
  delete tr, n, mu, c2, ratio1, ratio2;
  return 0;

}



