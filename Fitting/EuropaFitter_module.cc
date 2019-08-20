////////////////////////////////////////////////////////////////////////
// Class:       EuropaFitter
// Plugin Type: analyzer (art v2_09_02)
// File:        EuropaFitter_module.cc
//
// Generated at Mon Mar 19 01:17:36 2018 by vagrant using cetskelgen
// from cetlib version v3_01_03.
////////////////////////////////////////////////////////////////////////

#include <ctime>
#include <string>
#include "TF1.h"
#include "TH1.h"
#include "TVirtualFFT.h"
#include "TCanvas.h"
#include "TMatrixDSym.h"
#include "TFitResult.h"
#include "TStyle.h"
#include "TRandom3.h"

#include "EuropaFitterFunctions.hh"
#include "RootManager/RootManager.hh"

#include "TFile.h"

using std::string;
using std::cout;
using std::endl;
using namespace blinding;

class EuropaFitter  {
public:
  explicit EuropaFitter(); //fhicl::ParameterSet const & p);
  // The compiler-generated destructor is fine for non-base
  // classes without bare pointers or other resource use.
  ~EuropaFitter(){rootManager_->WriteToFile();}
  void beginJob() ;
  void endJob() ;

  // Plugins should not be copied or assigned.
  EuropaFitter(EuropaFitter const &) = delete;
  EuropaFitter(EuropaFitter &&) = delete;
  EuropaFitter & operator = (EuropaFitter const &) = delete;
  EuropaFitter & operator = (EuropaFitter &&) = delete;

  // Required functions.
  void analyze() ;

private:

  // Declare member data here.
  std::unique_ptr<TFile> outputRootFile_;
  std::unique_ptr<RootManager> rootManager_;

  std::string name_;
  std::vector<std::string> fitLabels_;
  double fitStartTime_;
  double fitEndTime_;

  // Procedural functions.
  TH1F *LoadFitData();
  TF1 *LoadFitFunc(std::string fitLabel, TH1F *data);
  void ApplyCorrections(TH1F *data);
  void RunFitRoutine(std::string fitLabel, TH1F *data, TF1 *f1);

  // Helper functions.
  void Calculate5ParGuesses(TH1F *data, std::vector<double> &guess);
  std::vector<double> GetPerfectParams();
  void SetFitParLimits(std::string fitLabel, TF1 *f1);

  void MakeDiffAndPullPlots(std::string fitLabel, TH1F *data);
  void MakeTimeSweepPlots(std::string fitLabel, TH1F *data);
  void MakeCrossValidationPlots(std::string fitLabel, TH1F *data);
    
  TH1F* GetResidualHist(TH1F* data, TF1* fit, 
			double min, double max, TH1F* pull);
  TH1F* GetResidualHist(TH1F* data, TH1F* hist, 
			double min, double max, TH1F* pull);
  TH1* FFTHisto(TH1F* hist);
  TH1F* SetupFFT(TH1* h, double xmin, double xmax);
  TH1F* RescaleAxis(TH1* input, Double_t Scale);
  void checkHists(TH1F* data, TH1F* cor);

  TFile* inputFile_;
  string inputFileName_;
  string inputPath_; //path to data hist

  TFile* correctionsFile_;
  string correctionsFileName_;

  bool usePseudoData_;
  bool calculate5ParGuesses_;

  std::map<std::string, double> fitParamMap_;
  bool usePreviousFitParams_;
  bool performStartTimeSweep_;
  bool performCrossValidation_;
};


EuropaFitter::EuropaFitter() //fhicl::ParameterSet const & p)
  :
  name_("EuropaFitter")
  , fitLabels_(/*"fitLabels"*/)
  , fitStartTime_(30.0)
  , fitEndTime_(650.0)
  , inputFileName_("plots_60hr_15922.root")
  , inputPath_("") //directory containing input plot
  , correctionsFileName_("corrections.root")
  , usePseudoData_(false)
  , calculate5ParGuesses_(true)
  , usePreviousFitParams_(true)
  , performStartTimeSweep_(false)
  , performCrossValidation_(false)
{
  // Set root style option for fits.
  gStyle->SetOptFit(11111);

  std::cout << "constructor \n";

  if (fitLabels_.size() == 0) {
    std::cout << "No fits specified, running 5-par fit."<<std::endl;
    fitLabels_ = {"5par"};
  }

  mapFitFunctions();
}


void EuropaFitter::beginJob()
{
  std::cout << "beginJob \n";
  
  //load in root file for reading data
  cout<<"Load input file "<<inputFileName_<<endl;
  inputFile_ = new TFile(inputFileName_.c_str(), "READ");

  //load in root file for reading corrections
  cout<<"Load corrections file "<<correctionsFileName_<<endl;
  correctionsFile_ = new TFile(correctionsFileName_.c_str(), "READ");
 
  // make root file for output
  TFile *outputRootFile_ = new TFile("blindedFitting.root", "RECREATE");
  rootManager_.reset(new RootManager("FittingPlots", outputRootFile_));

  for (auto &fitLabel : fitLabels_) {
    auto dir = rootManager_->GetDir(fitLabel,true);
    cout << "booking dir: " << dir->GetName() << "\n";

    if (usePseudoData_){
      // histograms for the pull of each parameter
      TH1F* diff1 = new TH1F("diff1", ";difference in lifetime (us)", 101, -0.5, 0.5);
      TH1F* diff2 = new TH1F("diff2", ";difference in amplitude", 101, -0.01, 0.01);
      TH1F* diff3 = new TH1F("diff3", ";difference in omega (blinded) ppm", 101, -20., 20);
      TH1F* diff4 = new TH1F("diff4", ";difference in phase", 101, -0.01, 0.01);
      
      //TH1F* pull0 = new TH1F("pull0", "", 70, -7, 7);
      TH1F* pull1 = new TH1F("pull1", "", 71, -7, 7);
      TH1F* pull2 = new TH1F("pull2", "", 71, -7, 7);
      TH1F* pull3 = new TH1F("pull3", "", 710, -70, 70); //increase range here for when blinding is on
      TH1F* pull4 = new TH1F("pull4", "", 71, -7, 7);
      
      //      rootManager_->Add(dir, pull0);
      rootManager_->Add(dir, diff1);
      rootManager_->Add(dir, diff2);
      rootManager_->Add(dir, diff3);
      rootManager_->Add(dir, diff4);
      
      rootManager_->Add(dir, pull1);
      rootManager_->Add(dir, pull2);
      rootManager_->Add(dir, pull3);
      rootManager_->Add(dir, pull4);
    }
  }
  
}


void EuropaFitter::endJob() {}


void EuropaFitter::analyze()
{
  cout << "analyze() \n";

  TH1F *data = nullptr;
  TF1 *func = nullptr;

  for (auto &fitLabel : fitLabels_) {

    data = LoadFitData();
    func = LoadFitFunc(fitLabel, data);
    ApplyCorrections(data);

    RunFitRoutine(fitLabel, data, func);
    MakeDiffAndPullPlots(fitLabel, data);
    MakeTimeSweepPlots(fitLabel, data);
    MakeCrossValidationPlots(fitLabel, data);
  }
}


TH1F *EuropaFitter::LoadFitData()
{
  cout << "loading input data from "<<inputPath_<<endl;
  //  TDirectory* inputDir = (TDirectory*)inputFile_->Get(inputPath_.c_str());
  // inputDir->ls();

  //load in data - for now use pseudo data
  string dataHistName = (usePseudoData_)? Form("pseudoData") : "data";

  cout << "getting data plot: "<<dataHistName<<endl;
  //  return (TH1F*)inputDir->Get(inputPath+"/"+dataHistName.c_str())->Clone();
  return (TH1F*)inputFile_->Get( (dataHistName).c_str())->Clone();
}


TF1 *EuropaFitter::LoadFitFunc(std::string fitLabel, TH1F *data)
{
  cout << "loading fitting function \n";

  auto fcn = fitFuncMap[fitLabel];

  //fitting function
  TF1 *f1 = new TF1("f1", fcn.func, 
	       fitStartTime_, fitEndTime_, 
	       fcn.parGuesses.size());

  for (uint pi = 0; pi < fcn.parNames.size(); ++pi) {
    f1->SetParName(pi, fcn.parNames[pi].c_str());
  }

  // number of pixels must be high for a decent display
  f1->SetNpx(2000);

  // Load the guesses
  auto guess = fcn.parGuesses;

  if (calculate5ParGuesses_) {
    Calculate5ParGuesses(data, guess);
  }

  // Take value from previous fits.
  if (usePreviousFitParams_) {

    for (uint pi = 0; pi < fcn.parNames.size(); ++pi) {

      auto &p = fcn.parNames[pi];
      std::cout << "Checking for previous " << p << std::endl;
      
      if (fitParamMap_.find(p) != fitParamMap_.end()) {
	guess[pi] = fitParamMap_[p];
      }
    }
  }

  int idx = 0;
  for (auto &g : guess) {
    std::cout << "guess[" << idx++ << "]: " << g << std::endl;
  }

  for (uint pi = 0; pi < guess.size(); ++pi) {
    f1->FixParameter(pi, guess[pi]);
  }

  return f1;
}


std::vector<double> EuropaFitter::GetPerfectParams()
{
  std::vector<double> perfect(5, 0.0);

  // Return an empty vector if we aren't in this mode.
  if (!usePseudoData_) {
    return perfect;
  }

  cout << "loading ideal fitting function \n";  
  
  TF1* perfectWiggle = NULL;
  TH1F* perfectData = NULL;
  
  TDirectory* inputDir = (TDirectory*)inputFile_->Get(inputPath_.c_str());

  perfectWiggle = (TF1*)inputDir->Get("wiggle");
  perfectData = (TH1F*)inputDir->Get("perfect")->Clone();
  
  //based on the new times get the perfect parameters
  double binWidth = perfectData->GetBinWidth(1);
  double lowBin = perfectData->GetBinLowEdge(perfectData->FindBin(fitStartTime_));
  double norm = perfectData->GetBinContent(perfectData->FindBin(fitStartTime_));
  norm /= perfectWiggle->Integral(lowBin, lowBin + binWidth);
    
  perfect[0] = norm * perfectWiggle->Eval(0); //this is incorrect...
  perfect[1] = perfectWiggle->GetParameter(1); 

  double scaleFactor = 1E10 / perfectWiggle->Integral(0., 700.);

  std::cout << "perfect wiggle integral first bin: "
	    << perfectWiggle->Integral(lowBin, lowBin + binWidth) << "\n";
  
  std::cout << "perfect hist bin content: "
	    << perfectData->GetBinContent(perfectData->FindBin(fitStartTime_)) << "\n";
  
  std::cout << "norm: "<< norm << "\n";
  std::cout << "scaleFactor: "<< scaleFactor << "\n";
  std::cout << "perfectWiggle eval at 0: "<< perfectWiggle->Eval(0) << "\n";
  std::cout << "perfectWiggle par 3: " << perfectWiggle->GetParameter(0) << "\n";
  perfect[2] = perfectWiggle->GetParameter(2);
  perfect[3] = perfectWiggle->GetParameter(3);
  perfect[3] = perfect[3] - blinder_.referenceValue();
  double perfectPeriod = TMath::TwoPi() / blinder_.referenceValue();
  double moduloPhase = fmod(TMath::TwoPi() * (fitStartTime_/perfectPeriod), TMath::TwoPi()); 

  perfect[4] = fmod( perfectWiggle->GetParameter(4) + moduloPhase, TMath::TwoPi());

  cout << "perfect params have been loaded \n";  

  return perfect;
}


void EuropaFitter::ApplyCorrections(TH1F *data)
{
  //load in corrections
  TH1F* corLostMuon = (TH1F*) correctionsFile_->Get("LostMuon");
  TH1F* corCBO = (TH1F*) correctionsFile_->Get("RadialCBO");
  TH1F* corVBO = (TH1F*) correctionsFile_->Get("VerticalBO");

  std::cout << "loaded correction hists " << corLostMuon->GetBinWidth(1) 
		     << ", " << corCBO->GetBinWidth(1) 
		     << ", " << corVBO->GetBinWidth(1) << "\n";

  //check bin sizes and hist ranges are the same
  //checkHists(data, corLostMuon);
  //checkHists(data, corCBO);
  //checkHists(data, corVBO);

  //apply the corrections
}  


void EuropaFitter::SetFitParLimits(std::string fitLabel, TF1 *f1)
{
  auto fcn = fitFuncMap[fitLabel];

  // now release parameters to perform fit.
  int idx = 0;
  for (auto &l : fcn.parLimits) {
    if ((l.first != l.second) && (l.first < l.second)) {
      std::cout << idx << ": [" << l.first << ", " << l.second << "]\n";
      f1->SetParLimits(idx++, l.first, l.second);
    } else {
      f1->ReleaseParameter(idx++);
    }
  }
}


void EuropaFitter::RunFitRoutine(std::string fitLabel, TH1F *data, TF1 *f1)
{
  //for writing
  cout<<fitLabel<<endl;
  auto fitDir = rootManager_->GetDir(fitLabel, true);
  auto perfect = GetPerfectParams();

  TH1F* dataTmp = (TH1F*) data->Clone();
  dataTmp->SetName(Form("initialGuess"));
  dataTmp->Fit("f1", "RQ");
  rootManager_->Add(fitDir, dataTmp);

  // Perform a fit with previously fit parameters fixed.
  SetFitParLimits(fitLabel, f1);

  if (usePreviousFitParams_) {

    for (int pi = 0; pi < f1->GetNpar(); ++pi) {

      auto p = std::string(f1->GetParName(pi));
      auto val = f1->GetParameter(pi);
      
      if (fitParamMap_.find(p) != fitParamMap_.end()) {
	f1->FixParameter(pi, val);
      }
    }

    // Fit with fixed parameters.
    data->Fit("f1", "RQ");
  }

  // Set parameter limits or release parameters.
  SetFitParLimits(fitLabel, f1);
  TFitResultPtr result = data->Fit("f1", "RSLL");
  TMatrixDSym covMatrix = result->GetCovarianceMatrix();

  std::cout << std::endl;
  for (int pi = 0; pi < f1->GetNpar(); ++pi) {
    // Print the value.
    std::cout << f1->GetParName(pi) << " fit: " 
	      << f1->GetParameter(pi) << " true: "
	      << perfect[pi] << std::endl;

    // And store the value for future fits.
    fitParamMap_[f1->GetParName(pi)] = f1->GetParameter(pi);
  }
  
  double I = data->Integral(data->FindBin(fitStartTime_), 
			    data->FindBin(fitEndTime_));

  std::cout << "total entries: " << I << "\n" << "\n";

  // for now just set the error from the diagonal terms, worry about correlations later
  std::vector<double> err;
  for (int i(0); i < f1->GetNpar(); i++){
    err.push_back(0.0);
    for (int j(0); j < f1->GetNpar(); j++){
      if (!(i == j)) continue;
      err[i] += covMatrix[i][j];
    }
  }
    
  std::cout << "\n";
  covMatrix.Print();
  for (int i(0); i < f1->GetNpar(); i++) {
    std::cout << "err " << i << ": " << sqrt(err[i]) 
	      << " from fit:" << f1->GetParError(i) << "\n";
  }
}


void EuropaFitter::MakeDiffAndPullPlots(std::string fitLabel, TH1F *data)
{
  
  auto fitDir = rootManager_->GetDir(fitLabel, true);
  auto f1 = data->GetFunction("f1");
  auto perfect = GetPerfectParams();

  if (usePseudoData_) {
    
    rootManager_->Get<TH1F*>(fitDir, "diff1")->Fill( (f1->GetParameter(1) - perfect[1]) );
    rootManager_->Get<TH1F*>(fitDir, "diff2")->Fill( (f1->GetParameter(2) - perfect[2]) );
    rootManager_->Get<TH1F*>(fitDir, "diff3")->Fill( (f1->GetParameter(3) - perfect[3]) );
    rootManager_->Get<TH1F*>(fitDir, "diff4")->Fill( (f1->GetParameter(4) - perfect[4]) );

    rootManager_->Get<TH1F*>(fitDir, "pull1")->Fill( (f1->GetParameter(1) - perfect[1]) / f1->GetParError(1) );
    rootManager_->Get<TH1F*>(fitDir, "pull2")->Fill( (f1->GetParameter(2) - perfect[2]) / f1->GetParError(2) );
    rootManager_->Get<TH1F*>(fitDir, "pull3")->Fill( (f1->GetParameter(3) - perfect[3]) / f1->GetParError(3) );
    rootManager_->Get<TH1F*>(fitDir, "pull4")->Fill( (f1->GetParameter(4) - perfect[4]) / f1->GetParError(4) );
  }
  
  rootManager_->Add(fitDir, data);

  TH1F* fitPull = new TH1F("pull", "", 141, -7, 7);
  
  TH1F* residual = GetResidualHist(data, 
				   data->GetFunction("f1"), 
				   fitStartTime_, 
				   fitEndTime_, 
				   fitPull);

  residual->SetName("fitResidual");
  rootManager_->Add(fitDir, residual);

  fitPull->SetName("fitPull");
  rootManager_->Add(fitDir, fitPull);
  
  //get FFT of residual
  TH1 *hm = 0;
  TVirtualFFT::SetTransform(0);

  TH1F* fftResidualInit = SetupFFT(residual, fitStartTime_, fitEndTime_);
  hm = fftResidualInit->FFT(hm,"MAG");

  TH1F* fftResidual = RescaleAxis(hm,1./(fitEndTime_ - fitStartTime_));

  fftResidual->SetTitle(";Frequency (MHz);Magnitude [Arb Units]");
  fftResidual->SetStats(0);
  fftResidual->SetName("residualFFT");
  fftResidual->Scale(1.0 / fftResidual->Integral());

  rootManager_->Add(fitDir, fftResidual);
}


void EuropaFitter::MakeTimeSweepPlots(std::string fitLabel, TH1F *data) 
{
  if (!performStartTimeSweep_) {
    return;
  }

  auto dirname = fitLabel + std::string("/startTimeSweep");
  auto dir = rootManager_->GetDir(dirname, true);
  auto f1 = data->GetFunction("f1");

  int nfits = 20;
  double tf = fitStartTime_ + 10.0;
  double dt = 10.0 / nfits;

  // Set up histograms.
  std::map<std::string, TH1F *> histMap;
  
  for (int pi = 0; pi < f1->GetNpar(); ++pi) {
    std::string par(f1->GetParName(pi));
    std::string hname = std::string("tfit_sweep_") + par; 

    double ibin = fitStartTime_ - 0.5 * dt;
    double fbin = tf - 0.5 * dt;
    histMap[par] = new TH1F(hname.c_str(), "", nfits, ibin, fbin);
  }

  // Vary the start time over a few omega-a cycles.
  int idx = 1;
  for (double ti = fitStartTime_; ti < tf; ti += dt) {
    auto tmp = (TH1F *)data->Clone();
    tmp->Fit("f1", "QRSLL", "", ti, fitEndTime_);
    
    for (int pi = 0; pi < f1->GetNpar(); ++pi) {

      double val = tmp->GetFunction("f1")->GetParameter(pi);
      double err = tmp->GetFunction("f1")->GetParError(pi);

      std::string par(f1->GetParName(pi));
      histMap[par]->SetBinContent(idx, val);
      histMap[par]->SetBinError(idx, err);
    }
    ++idx;

    if (idx == 9) {
           rootManager_->Add(dir, tmp);
    }
  }

  for (auto &h : histMap) {
    rootManager_->Add(dir, h.second);
  }
}


void EuropaFitter::MakeCrossValidationPlots(std::string fitLabel, TH1F *data) 
{
  if (!performCrossValidation_) {
    return;
  }

  // Save the randomon generator to keep the seed over multiple uses.
  static TRandom3 gen(clock());

  auto dirname = fitLabel + std::string("/crossValidation");
  auto dir = rootManager_->GetDir(dirname, true);
  auto f1 = data->GetFunction("f1");

  // Set up histograms.
  std::map<std::string, TH1F *> histMap;
  int nbins = 20;
  int ntimes = 100;
  int k = 20;
  int s = 2;

  for (int pi = 0; pi < f1->GetNpar(); ++pi) {
    std::string par(f1->GetParName(pi));
    std::string hname = std::string("cv_") + par; 

    double xlo = f1->GetParameter(pi) - s * f1->GetParError(pi);
    double xhi = f1->GetParameter(pi) + s * f1->GetParError(pi);
    histMap[par] = new TH1F(hname.c_str(), "", nbins, xlo, xhi);
  }

  // Zero a random set of bins from the histogram.
  for (int i = 0; i < ntimes; ++i) {

    auto tmp = (TH1F *)data->Clone();

    for (int ki = 0; ki < k; ++ki) {
      int val = gen.Integer(tmp->GetXaxis()->GetNbins());
      tmp->SetBinContent(val, 0.0);
      tmp->SetBinError(val, 1e100); // Effectively remove bin from fit.
    }
    
    tmp->Fit("f1", "QRS", "", fitStartTime_, fitEndTime_);

    for (int pi = 0; pi < f1->GetNpar(); ++pi) {

      double val = tmp->GetFunction("f1")->GetParameter(pi);
      //double err = tmp->GetFunction("f1")->GetParError(pi);

      std::string par(f1->GetParName(pi));
      histMap[par]->Fill(val);
      //histMap[par]->SetBinError(idx, err);
    }
  }

  for (auto &h : histMap) {
    rootManager_->Add(dir, h.second);
  }
}


void EuropaFitter::Calculate5ParGuesses(TH1F *data, std::vector<double> &guess)
{
  if (guess.size() < 5) {
    guess.resize(5);
  }

  //for guesses loop over pseudo hist until we have been over 5 maximums
  int nmax = 0;
  double timeMax[5] = {0.,0.,0.,0.,0.};
  double yMax[5] = {0.,0.,0.,0.,0.};

  int ibin = data->FindBin(fitStartTime_)+5;
  //  double prevTime = data->GetBinCenter(ibin-1);
  // double prevCont1 = data->GetBinContent(ibin-1);
  //double prevCont2 = data->GetBinContent(ibin-2);
  double prevTime = data->GetBinCenter(ibin-1);
  double prevCont1 = data->GetBinContent(ibin-1);
  double prevCont2 = data->GetBinContent(ibin-2);

  while (nmax < 5){
    double cont = data->GetBinContent(ibin);
    double time = data->GetBinCenter(ibin);
    
    //only look when we were approaching a maximum (because we are using a cosine)
    if (prevCont1 - prevCont2 > 0) {
      
      //find a turning point when the signs of the differences are not equal
      if ( (prevCont1 - cont) * (prevCont2 - prevCont1) < 0) {
	timeMax[nmax] = prevTime;
	yMax[nmax] = prevCont1;
	nmax++;
      }
    }
    prevTime = time;
    prevCont2 = prevCont1;
    prevCont1 = cont;
    ibin++;
  }

  //we have 4 full cycles so can guess period
  double period = (timeMax[4] - timeMax[0]) / 4.0;
  
  //get time and y value of minimum (at half a period later from peak)
  double tLow = timeMax[0] + (period/ 2.0);
  double yLow = data->GetBinContent( data->FindBin(tLow) ); 

  // can guess decay constant based on time and amplitude
  guess[1] = 4 * period / log (yMax[0]/yMax[4]);
  
  //determine the frequency from the period
  guess[3] = 0; 
  
  //determine the phase based on the period and the time of the first peak
  double timeOfFirstPeak = timeMax[0] - fitStartTime_;
  guess[4] = TMath::TwoPi() *( 1 - (timeOfFirstPeak / period)); 
  
  //determine the amplitude based in the average of the first max and min, and correct for decay
  guess[0] = (yMax[0] + yLow) / 2.0; 
  guess[0] *= exp(timeMax[0] / guess[1]);
  
  //can guess amplitude by looking at y at time of first peak and half a period later, need to know N0
  guess[2] = (yMax[0] - yLow) / (2 * guess[0]);
}


TH1* EuropaFitter::FFTHisto(TH1F* hist) {

  // FFT of the rms plot to get frequency for modulo plot                                                                                                                       
  TH1* hist_FFT = 0;
  TVirtualFFT::SetTransform(0);
  hist_FFT = (TH1*)hist->FFT(hist_FFT,"MAG");

  int nbins = hist_FFT->GetNbinsX();
  double binw  = hist->GetBinWidth(1);
  double freq = 1/binw;

  double xfft1 = hist_FFT->GetXaxis()->GetXmin();
  double xfft2 = hist_FFT->GetXaxis()->GetXmax();

  TH1F* hist_FFT_scale = new TH1F("hfft","",nbins/2 + 1,xfft1,xfft2);
  for (int i(1); i<=( nbins/2 + 1); i++) {
    double y0 = (hist_FFT->GetBinContent(i) - hist_FFT->GetBinContent(nbins+1 - i));
    double y = sqrt(y0*y0);
    double ynew = y/sqrt(nbins);
    double x = hist_FFT->GetXaxis()->GetBinCenter(i);
    hist_FFT_scale->Fill(x,ynew);
  }

  hist_FFT->Delete();
  hist_FFT_scale->GetXaxis()->SetLimits(0, freq);

  return hist_FFT_scale;

}

TH1F* EuropaFitter::SetupFFT(TH1* h, double xmin, double xmax){
  double timeTot = xmax - xmin;
  double binW = h->GetBinWidth(1);
  int nBins = timeTot / binW;
  TH1F* hout = new TH1F("","",nBins, xmin, xmin + (nBins*binW));

  int binCount = 0;
  for (int i(0); i < h->GetXaxis()->GetNbins(); i++){
    if (h->GetBinCenter(i) < xmin ) continue;
    if (binCount > nBins) break;
    binCount++;
    double cont = h->GetBinContent(i);
    double err = h->GetBinError(i);
    hout->SetBinContent(binCount, cont);
    hout->SetBinError(binCount, err);
  }
  return hout;
}

TH1F* EuropaFitter::RescaleAxis(TH1* input, Double_t Scale) {
  int bins = input->GetNbinsX();
  TAxis* xaxis = input->GetXaxis();
  double* ba = new double[bins+1];
  xaxis->GetLowEdge(ba);
  ba[bins] = ba[bins-1] + xaxis->GetBinWidth(bins);
  for (int i = 0; i < bins+1; i++) {
    ba[i] *= Scale;
  }
  TH1F* out = new TH1F(input->GetName(), input->GetTitle(), bins, ba);
  for (int i = 0; i <= bins; i++) {
    out->SetBinContent(i, input->GetBinContent(i));
    out->SetBinError(i, input->GetBinError(i));
  }
  return out;
}

TH1F* EuropaFitter::GetResidualHist(TH1F* data, TF1* fit, double min, double max, TH1F* pull){

  int nbins = data->GetXaxis()->GetNbins();
  double binWidth = data->GetBinWidth(1);
  double low = data->GetXaxis()->GetBinLowEdge(1);
  double high = low + nbins*binWidth;
  TH1F* residual = new TH1F("residual", "", nbins, low, high);  

  for (int ibin(1); ibin <= nbins; ibin++){
    residual->SetBinContent(ibin, 0.0);
    double time = residual->GetXaxis()->GetBinCenter(ibin);
    if (time > max) break;
    if (time >= min) {
      double cont = data->GetBinContent(ibin);
      double err = data->GetBinError(ibin);
      //double integral = fit->Integral( data->GetBinLowEdge(ibin), data->GetBinLowEdge(ibin) + binWidth );
      double integral = fit->Eval(time);
      residual->SetBinContent(ibin, integral - cont);
      residual->SetBinError(ibin, err);
      pull->Fill( (integral - cont) / err);
    }
  }
  return residual;
}

TH1F* EuropaFitter::GetResidualHist(TH1F* data, TH1F* perfect, double min, double max, TH1F* pull){
  int nbins = data->GetXaxis()->GetNbins();
  double binWidth = data->GetBinWidth(1);
  double low = data->GetXaxis()->GetBinLowEdge(1);
  double high = low + nbins*binWidth;
  TH1F* residual = new TH1F("residual", "", nbins, low, high);
  
  for (int ibin(1); ibin <= nbins; ibin++){
    residual->SetBinContent(ibin, 0.0);
    double time = residual->GetXaxis()->GetBinCenter(ibin);
    if (time > max) break;
    if (time > min) {
      double cont = data->GetBinContent(ibin);
      double err = data->GetBinError(ibin);
      double integral = perfect->GetBinContent(ibin);
      residual->SetBinContent(ibin, integral - cont);
      pull->Fill( (integral - cont) / err);
    }
  }
  return residual;
}

void EuropaFitter::checkHists(TH1F* data, TH1F* cor)
{
  if (data->GetXaxis()->GetNbins() != cor->GetXaxis()->GetNbins()) {
    throw  (std::string)cor->GetName() + " hist bins configured incorrectly \n";
  }

  if (data->GetXaxis()->GetBinCenter(1) != cor->GetXaxis()->GetBinCenter(1)) {
    throw  (std::string)cor->GetName() + " 1st bin configured incorrectly \n";
  }
}





//==============================================
//==============================================
//==============================================
//==============================================

int main() {
  EuropaFitter *a = new EuropaFitter();
  a->beginJob();
  a->analyze();
  a->endJob();
  delete a;
  return 1;
}
