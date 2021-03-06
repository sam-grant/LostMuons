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
      sfMu = 1.49;//2.75606;// 1.46936;// +/- 0.0533611 
    }
    else if ( stn == 18 ) {
      sfMu = 1.60;//72981;//1.56875;// +/- 0.0385335 
    }
  
    string h1 = "St"+to_string(stn)+"_radial";
    string h2 = "St"+to_string(stn)+"_radial_2";

    string fname1;

    if (scale) {
      fname1 = "Plots/St"+to_string(stn)+"RadialScaled.png";
    }
    else if (!scale) { 
      fname1 = "Plots/St"+to_string(stn)+"RadialUnscaled.png";
    }

    string title1 = "Stn "+to_string(stn)+";Radial Decay Vertex [mm];Entries";
 
    // string title2 = "Stn "+to_string(stn)+" | Sanity Check";

    TFile *f1 = TFile::Open("../ReadOldNTuples/Track/RootFiles/Track.root");
    TFile *f2 = TFile::Open("../ReadOldNTuples/TrackCluster/RootFiles/TrackCluster.root");
 
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
    tr->SetName("All tracks");
    
    if (scale) {
      subMu->SetName("All tracks #minus lost muons");
      mu->SetName("Lost muons (scaled)");
    }

    else if (!scale) {
      subMu->SetName("All tracks #minus lost muons");
      mu->SetName("Lost muons (unscaled)");
    }

    f1->Save();
    f2->Save();
   
    // Draw first histogram
    tr -> Draw();
    gStyle->SetOptStat(2201);
    gPad->Update();
    
    // Collect stats of the first histogram 
    TPaveStats *tps1 = (TPaveStats*) tr -> FindObject("stats"); 

    tps1->SetTextColor(kBlue);
    tps1->SetLineColor(kBlue);

    tps1->SetX1NDC(0.70);
    tps1->SetX2NDC(0.89);
    tps1->SetY1NDC(0.69);
    tps1->SetY2NDC(0.89);

    double X1 = tps1->GetX1NDC();
    double Y1 = tps1->GetY1NDC();
    double X2 = tps1->GetX2NDC();
    double Y2 = tps1->GetY2NDC();

    // Draw second histogram
    subMu -> Draw();
    gStyle->SetOptStat(2201);
    gPad->Update();

    // Collect stats of the second histogram
     
    TPaveStats *tps2 = (TPaveStats*) subMu -> FindObject("stats");
  
    tps2->SetTextColor(kRed);
    tps2->SetLineColor(kRed);

    // Put stat box beneath the first
    // Put stat box beneath the first
    tps2->SetX1NDC(X1);
    tps2->SetX2NDC(X2);
    tps2->SetY1NDC((Y1-0.025)-(Y2-Y1));
    tps2->SetY2NDC(Y1-0.025);

    // Draw third histogram
    /* mu -> Draw(); */
    /* gStyle->SetOptStat(2211); */
    /* gPad->Update(); */

    // Collect stats of the third histogram

    /* TPaveStats *tps3 = (TPaveStats*) mu -> FindObject("stats"); */
 
    /* tps3->SetTextColor(kGreen-3); */
    /* tps3->SetLineColor(kGreen-3); */

    /* // Put stat box beneath the first */
    /* tps3->SetX1NDC(X1); */
    /* tps3->SetX2NDC(X2);  */
    /* tps3->SetY1NDC(Y1-2*(Y2-Y1));  */
    /* tps3->SetY2NDC(Y1-(Y2-Y1)); */
    
    /* // Draw histograms on one canvas */

    TCanvas *c1 = new TCanvas("","",3000,3000);
    gStyle->SetStatFormat("6.3g");
    c1->Divide(1,2);

    c1->cd(1);

    tr->SetMarkerColor(kBlue);
    tr->SetLineColor(kBlue);
    subMu->SetMarkerColor(kRed);
    subMu->SetLineColor(kRed);
    tr->SetLineWidth(5);
    subMu->SetLineWidth(5);
    // subN->SetMarkerColor(kBlue+2);
    //  subN->SetLineColor(kBlue+2);
    mu->SetMarkerColor(kGreen+2);
    mu->SetLineColor(kGreen+2);
    tr->GetXaxis()->SetRangeUser(-150,150);
    tr->SetTitle(title1.c_str());
    //    c1->cd(1)->SetLogy();
    tr->Draw();
    subMu->Draw("same");
    // mu->Draw("same");
    tps1->Draw("same"); 
    tps2->Draw("same");
    //tps2->Draw("same");
    tps1->SetLineWidth(5);
    tps2->SetLineWidth(5);

    c1->cd(2);

    TH1D *ratio = (TH1D*)subMu->Clone("ratio");

    ratio->Divide(tr);
    ratio->SetStats(0);
    //    ratio->SetTitle("(All tracks #minus lost muons) / all tracks");
    ratio->SetMarkerColor(kBlack);
    ratio->SetLineColor(kBlack);
    ratio->SetLineWidth(5);
    //    ratio->GetYaxis()->SetRangeUser(0.4,1.5);
    ratio->GetYaxis()->SetTitle("Ratio");
    ratio->GetXaxis()->SetRangeUser(-150,150);
    ratio->GetXaxis()->SetTitle("Radial Decay Vertex [mm]");
    gPad->SetGridy();
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
    cout<<"St"<<stn<<" dRMS = "<<dRMS<<"+/-"<<dRMS_err<<"\n"<<endl;
    
    cout<<stn<<","<<dMean<<","<<dMean_err<<","<<dRMS<<","<<dRMS_err<<endl;
       
  }
  return 0;
} 
