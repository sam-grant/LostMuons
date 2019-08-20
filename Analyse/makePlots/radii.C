//Code to make tracker radii plots and subtracted radii plots
// Sam Grant - March 2019
#include <iostream>
#include "TFile.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TPaveStats.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TLatex.h"

using namespace std;

int main() {

  // Apply scaling? 
  // s = 0 scale=true ; s = 1 scale=false
  bool scale = false; 
  string input;
  cout << "Scaled? Yes (y) or no (n): ";
  cin >> input;
  if (input == "y") { scale = true; }
  else if (input == "n" ) { scale = false; }
  else {
    cout << "Error, try again" << endl;
    return 0;
  }

 

  for (int stn = 12 ; stn<19 ; stn = stn + 6) {

    double sfMu;
  
    // Hard code  scale factors
    if ( stn == 12 ) {
      sfMu =2.75606;// 1.46936;// +/- 0.0533611 
    }
    else if ( stn == 18 ) {
      sfMu = 1.72981;//1.56875;// +/- 0.0385335 
    }
  
    string h1 = "St"+to_string(stn)+"_radial";
    string h2 = "St"+to_string(stn)+"_radial_2";

    string fname1;
    // string fname2 = "../plots/stn"+to_string(stn)+"_tracker_radius_check.pdf";

    if (scale) {
      fname1 = "../plots/stn"+to_string(stn)+"_tracker_radius_subtracted.png";
    }
    else if (!scale) { 
      fname1 = "../plots/stn"+to_string(stn)+"_tracker_radius_subtracted_noscale.png";
    }

    string title1 = "Stn "+to_string(stn)+" | Radial Decay Vertices;Radial Decay Vertex [mm];N";
 
    // string title2 = "Stn "+to_string(stn)+" | Sanity Check";

    TFile *f1 = TFile::Open("../tracker/merged_tracker_noVQ.root");
    TFile *f2 = TFile::Open("../cluster_master/merged_cluster_james.root");
 
    TH1D *tr = (TH1D*)f1->Get(h1.c_str());
    TH1D *mu = (TH1D*)f2->Get(h2.c_str());
 
    // Scala
    if (scale) {
      mu -> Scale(sfMu);
    }
 
    // Clone and subtract
    TH1D *subMu = (TH1D*)tr->Clone("subMu");
    subMu -> Add(mu,-1);

    // Rename for presentation
    tr->SetName("All Tracks");
    if (scale) {
      subMu->SetName("All Tracks  #minus Lost Muons  (Scaled)");
      mu->SetName("Lost Muons (Scaled)");
    }

    else if (!scale) {
      subMu->SetName("All Tracks  #minus Lost Muons  (Unscaled)");
      mu->SetName("Lost Muons (Unscaled)");
    }

    f1->Save();
    f2->Save();
   
    // Draw first histogram
    tr -> Draw();
    gStyle->SetOptStat(2211);
    gPad->Update();
    
    // Collect stats of the first histogram 
    TPaveStats *tps1 = (TPaveStats*) tr -> FindObject("stats"); 

    tps1->SetTextColor(kBlue+2);
    tps1->SetLineColor(kBlue+2);
    /*
    */

    tps1->SetX1NDC(0.80);
    tps1->SetX2NDC(0.99);
    tps1->SetY1NDC(0.75);
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
    mu -> Draw();
    gStyle->SetOptStat(2211);
    gPad->Update();

    // Collect stats of the third histogram

    TPaveStats *tps3 = (TPaveStats*) mu -> FindObject("stats");
 
    tps3->SetTextColor(kGreen+2);
    tps3->SetLineColor(kGreen+2);

    // Put stat box beneath the first
    tps3->SetX1NDC(X1);
    tps3->SetX2NDC(X2); 
    tps3->SetY1NDC(Y1-2*(Y2-Y1)); 
    tps3->SetY2NDC(Y1-(Y2-Y1));
    
    // Draw histograms on one canvas

    TCanvas *c1 = new TCanvas("c1","c1",2000,1000);

    c1->Divide(1,2);

    c1->cd(1);

    tr->SetMarkerColor(kBlue+2);
    tr->SetLineColor(kBlue+2);
    subMu->SetMarkerColor(kRed+2);
    subMu->SetLineColor(kRed+2);
    // subN->SetMarkerColor(kBlue+2);
    //  subN->SetLineColor(kBlue+2);
    mu->SetMarkerColor(kGreen+2);
    mu->SetLineColor(kGreen+2);
    tr->GetXaxis()->SetRangeUser(-150,150);
    tr->SetTitle(title1.c_str());
    c1->cd(1)->SetLogy();
    tr->Draw();
    subMu->Draw("same");
    mu->Draw("same");
    tps1->Draw("same"); 
    tps2->Draw("same");
    tps2->Draw("same");

    c1->cd(2);

    TH1D *ratio = (TH1D*)subMu->Clone("ratio");

    ratio->Divide(tr);
    ratio->SetStats(0);
    ratio->SetTitle("Ratio: (All Tracks  #minus Lost Muons) / All Tracks");
    ratio->SetMarkerColor(kBlack);
    ratio->SetLineColor(kBlack);
    ratio->GetYaxis()->SetRangeUser(0.4,1.5);
    ratio->GetYaxis()->SetTitle("Ratio");
    ratio->GetXaxis()->SetRangeUser(-150,150);
    ratio->GetXaxis()->SetTitle("Radial Decay Vertex [mm]");
    gPad->SetGrid();
    ratio->Draw();
    
    // tps3->Draw("same");  
    c1 -> SaveAs(fname1.c_str());
    delete c1;

    double dMean =  subMu->GetMean() - tr->GetMean();
    double dMean_err = sqrt( (subMu->GetMeanError())*(subMu->GetMeanError()) + (tr->GetMeanError())*(tr->GetMeanError()) );
    double dRMS =  subMu->GetRMS() - tr->GetRMS();
    double dRMS_err = sqrt( (subMu->GetRMSError())*(subMu->GetRMSError()) + (tr->GetRMSError())*(tr->GetRMSError()) ); 
    cout<<"******************* RESULT ******************* "<<endl;
    cout<<"St"<<stn<<" dMean = "<<dMean<<"+/-"<<dMean_err<<endl;
    cout<<"St"<<stn<<" dRMS = "<<dRMS<<"+/-"<<dRMS_err<<endl;
  }
  return 0;
} 
