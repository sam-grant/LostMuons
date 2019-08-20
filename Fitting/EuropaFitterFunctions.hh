#ifndef EUROPA_FITTER_FUNCTIONS_HH_
#define EUROPA_FITTER_FUNCTIONS_HH_

#include <utility>
#include <vector>
#include <map>

#include "blinders/Blinders.hh"
#include "TF1.h"

// Setup a blinder.
blinding::Blinders blinder_(blinding::Blinders::kOmega_a, "europa gavin");

// Use this struct to define fit functions.
struct wa_function_t {
  Double_t (*func)(Double_t *x, Double_t *p);
  std::vector<std::string> parNames;
  std::vector<double> parGuesses;
  std::vector<std::pair<double, double>> parLimits;
};

std::map<std::string, struct wa_function_t> fitFuncMap;

// Standard 5-par fit
Double_t fitFunc5Par(Double_t *x, Double_t *par){
  Double_t t = x[0];
  Double_t N = par[0];
  Double_t B = par[1];
  Double_t A = par[2];
  Double_t R = par[3];
  Double_t p = par[4];
  
  Double_t w = blinder_.paramToFreq(R);
  
  return N * exp(-t / B) * (1 + A * cos(w * t + p));
}

struct wa_function_t fit_func_5par = {
  fitFunc5Par, 
  {"N", "#tau", "A", "R", "#phi"}, 
  {1e9, 64.4, 0.4, 0.0, 0.0},
  {
    {-1.0, -1.0},
    {63.0, 66.0},
    {0.0, 1.0},
    {-100.0, 100.0},
    {0.0, 2*M_PI}
  }
};

// 9-par fit which includes CBO terms.
Double_t fitFunc9Par(Double_t *x, Double_t *par){
  Double_t t = x[0];
  Double_t N = par[0];
  Double_t B = par[1];
  Double_t A = par[2];
  Double_t R = par[3];
  Double_t p = par[4];
  Double_t ACBO = par[5];
  Double_t wCBO = par[6];
  Double_t pCBO = par[7];
  Double_t tCBO = par[8];
  
  Double_t w = blinder_.paramToFreq(R);
  Double_t CBOTerm = 1.0 - (exp(-t / tCBO) * ACBO * cos(wCBO * t+ pCBO));
    
  return N * exp(-t / B) * (1 + A * cos( w * t + p)) * CBOTerm;
}

struct wa_function_t fit_func_9par = {
  fitFunc9Par, 
  {"N", "#tau", "A", "R", "#phi",
   "A_{CBO}", "#omega_{CBO}", "#phi_{CBO}", "#tau_{CBO}"}, 
  {1e9, 64.4, 0.4, 0.0, 3.14, 0.004, 2.324, 3.14, 300.0},
  {
    {-1.0, -1.0},
    {63.0, 66.0},
    {0.0, 1.0},
    {-100.0, 100.0},
    {0.0, 2*M_PI},
    {-1.0, -1.0},
    { 2.3,  2.4},
    {0.0, 2*M_PI},
    {-1.0, -1.0}
  }
};

// Add to the map.			   
void mapFitFunctions() {
  fitFuncMap["5par"] = fit_func_5par;
  fitFuncMap["9par"] = fit_func_9par;
}

#endif
