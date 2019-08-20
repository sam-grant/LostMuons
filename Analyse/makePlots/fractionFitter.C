
//Code to find the fraction of events between tracker and cluster between 2.2 and 3.2 GeV 
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
  
  // No vertexQ
  string trackerFile = "../tracker/merged_tracker_noVQ.root";
  // Only track quality
  string clusterFile = "../cluster_master/merged_cluster_james.root";

  for (int stn = 12 ; stn < 19 ; stn = stn + 6) {

    cout << "##################################################" << endl;
    cout << "Stn " << stn << endl;
  
    string h1 = "St"+to_string(stn)+"_momentum";  // All tracks
    string h2 = "St"+to_string(stn)+"_momentum_1"; // Non Lost Muons
    string h3 = "St"+to_string(stn)+"_momentum_2"; // Lost Muons
  
    string fname1 = "../plots/St"+to_string(stn)+"fracFit2.png";
    string fname2 = "../plots/St"+to_string(stn)+"momRatio2.png";

    TFile *f1 = TFile::Open(trackerFile.c_str());
    TFile *f2 = TFile::Open(clusterFile.c_str());
     
    string title = "Stn "+to_string(stn)+" | Fractional Fit Region; Track Momentum [MeV]; N";
  
    TH1D *tr = (TH1D*)f1->Get(h1.c_str());
    TH1D *n = (TH1D*)f2->Get(h2.c_str());
    TH1D *mu = (TH1D*)f2->Get(h3.c_str());

    /*  tr->GetXaxis()->SetLimits(0,3.5);
    n->GetXaxis()->SetLimits(0,3.5);
    mu->GetXaxis()->SetLimits(0,3.5);*/

    tr->SetName("All Tracks (Tracker)");
    n->SetName("Non Lost Muons (Tracker & Calo)");
    mu->SetName("Lost Muons (Tracker & Calo)");  

    n->SetLineColor(kRed+2);
    n->SetMarkerColor(kRed+2);
    mu->SetLineColor(kGreen+2);
    mu->SetMarkerColor(kGreen+2);
    tr->SetMarkerColor(kBlue+2);
    tr->SetLineColor(kBlue+2);

    f1->Save();
    f2->Save(); 

    /////////////////////////////////////////////////////////////////////////////
    //Fractional Fit
    ////////////////////
   
    //Cluster histograms are put in this array                                                   
    TObjArray *cl = new TObjArray(3);  
    cl->Add(mu);
    cl->Add(n);

    // Initialise
    TFractionFitter* fit = new TFractionFitter(tr, cl);
    // Set X range
    int bin2300 = tr -> GetXaxis() -> FindBin(2300);
    int bin3000 = tr -> GetXaxis() -> FindBin(3000);
    fit->SetRangeX(bin2300, bin3000);//tr->GetXaxis()->FindBin(2300),tr->GetXaxis()->FindBin(3000));  

    Int_t status = fit->Fit(); //Perform the fit
    Double_t v0, v1, e0, e1; // Book fit parameters
    cout << "fit status: " << status << endl; // Check on fit status

    if (status == 0) {                      

      TH1D* result = (TH1D*) fit->GetPlot(); // Book result
      fit->GetResult(0,v0,e0);
      fit->GetResult(1,v1,e1);

      TCanvas *c1 = new TCanvas("c1", "c1", 2000, 1000);
      tr->SetStats(0);
      tr->GetYaxis()->SetMaxDigits(3);
      tr->GetXaxis()->SetRangeUser(0,3500);
      tr->Draw();
      cl->Draw("same");
      result->Draw("same");
      result->SetLineColor(kMagenta+2);
      c1->BuildLegend(0.65,0.75,0.89,0.89);
      tr->SetTitle(title.c_str());
      // Save 
      c1 -> SaveAs(fname1.c_str()); // Visualise the fit
      delete c1;
    }
    delete cl;

    // Find calo efficiency
    double I_tot = tr -> Integral(bin2300, bin3000);
    double I_mu = mu -> Integral(bin2300, bin3000);
    double I_n = n -> Integral(bin2300, bin3000);

    double eff_mu = I_mu / (I_tot  * v0 ); 
    double eff_mu_err = eff_mu * (e0/v0);
    double eff_n = I_n / (I_tot * v1);
    double eff_n_err = eff_n * (e1/v1);

    cout << "##################################################" << endl;
    cout << "Stn " << stn << endl;
    cout << "Lost Muon Fraction = "<<v0<<" +/- "<<e0<<endl;
    cout << "Non Lost Muon Fraction = "<<v1<<" +/- "<<e1<<endl;
    cout<< "Lost Muon Efficiency  = "<<eff_mu*100<<" +/- "<<eff_mu_err*100<<"%"<<endl;
    cout<< "Non Lost Muons Efficiency  = "<<eff_n*100<<" +/- "<<eff_n_err*100<<"%"<<endl;
    cout << "Scale Factor Lost Muons = " << 1./eff_mu <<" +/- "<<(1./eff_mu)*(eff_mu_err/eff_mu)<<endl;
    cout << "Scale Factor Non Lost Muons = " << 1./eff_n <<" +/- "<<(1./eff_n)*(eff_n_err/eff_n)<<endl;
    cout << "##################################################" << endl;

    // Add those suckers, do a cross check to make sure the cluster and tracker distributions line up at high p
  
    // Draw momentum distribution and ratios
  
    TH1D *ratio1 = (TH1D*)n->Clone("ratio1");
    TH1D *ratio2 = (TH1D*)mu->Clone("ratio2");

    ratio1->Divide(tr);
    ratio2->Divide(tr);

    TCanvas *c2 = new TCanvas("c2", "c2", 2000, 1000);

    c2->Divide(1,2);
    c2->cd(1);
    tr->Draw();
    n->Draw("same");
    mu->Draw("same");
    tr->SetTitle(0);
    tr->SetName("All Tracks (Tracker)");
    c2->cd(1)->BuildLegend(0.75,0.75,0.99,0.99);
    tr->SetStats(0);
    n->SetStats(0);
    mu->SetStats(0);
    tr->GetYaxis()->SetTitleOffset(0.75);
    tr->GetXaxis()->SetRange(bin2300,bin3000);
    tr->SetTitle(title.c_str());

    c2->cd(2);

    ratio1->SetName("All Tracks / Non Lost Muons");
    ratio1->GetYaxis()->SetTitleOffset(0.75);
    ratio1->GetYaxis()->SetTitle("Fraction");
    ratio1->SetStats(0);
    ratio1->SetLineColor(kMagenta+2);
    ratio1->SetMarkerColor(kMagenta+2);
    ratio1->GetXaxis()->SetRangeUser(2300,3000);
    ratio1->GetYaxis()->SetRangeUser(0,1);
    ratio1->Draw();
 
    ratio2->SetName("All Tracks / Lost Muons");
    ratio2->SetStats(0);
    ratio2->SetLineColor(kOrange+2);
    ratio2->SetMarkerColor(kOrange+2);
    ratio2->Draw("same");
    c2->SetGrid();
    c2->cd(2)->BuildLegend(0.765,0.82,0.99,0.99);
    c2->cd(2)->SetGrid();

    c2->SaveAs(fname2.c_str());

  }

  return 0;

}



