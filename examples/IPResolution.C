#include <iostream>
#include <utility>
#include <vector>
#include <map>


#include "TFile.h"
#include "TCanvas.h"
#include "TH1D.h"

std::map<int, TH1D* > storeIPResolution(TString fileName, std::vector<int> ptBins) {
  
  
  std::map<int, TH1D* > myMap;
  TFile* myFile = TFile::Open(fileName); // do not close!
  TString canvasName = "dxyres_pixel_MS000";
  TCanvas *c1 = (TCanvas*)myFile->Get("dxyres_pixel_MS000");
  std::vector<TH1D*> myTFs;
  for (std::vector<int>::iterator it_ptBins = ptBins.begin(); it_ptBins != ptBins.end(); ++it_ptBins) {
    TString histClone = canvasName + TString::Format("_%i", *it_ptBins);
    if (c1->GetPrimitive(TString::Format("d_vs_eta%ipixel_profile", *it_ptBins))) {
      TH1D* h1 = (TH1D*)(c1->GetPrimitive(TString::Format("d_vs_eta%ipixel_profile", *it_ptBins)));
      myMap[*it_ptBins] = (TH1D*)h1->Clone(histClone);
    }
    else {
      std::cout << TString::Format("d_vs_eta%ipixel_profile", *it_ptBins) << " does not exist" << std::endl;
    }
  }
  return myMap;
  
}

float getIPResolution(std::map<int, TH1D* > histMap, std::vector<int> ptBins, float ptValue, float etaValue) {
  
  int ptBin = 0;
  // loop over ptBins vector and find next-lowest bin
  for (std::vector<int>::iterator it_ptBins = ptBins.begin(); it_ptBins != ptBins.end(); ++it_ptBins) {
    if (ptValue > *it_ptBins) {
      ptBin = *it_ptBins;
    }
    else
      break;
  }
  histMap[ptBin]->Draw();
  
  Int_t binx = histMap[ptBin]->GetXaxis()->FindBin(fabs(etaValue));
  float resolution = histMap[ptBin]->GetBinContent(binx);
  return resolution;
  
}

void testCase() {
  
  std::vector<int> ptBins; // sorted ascending
  ptBins.push_back(1);
  ptBins.push_back(5);
  TString fileName = "/shome/vlambert/Phase2UpgradeStudies/tkgeometry/www/layouts/TechnicalProposal2014/dxyres_pixel_MS000.root";
  std::map<int, TH1D* > histMap = storeIPResolution(fileName, ptBins);
  std::cout << getIPResolution(histMap, ptBins, 15, 2.1) << std::endl;
  std::cout << getIPResolution(histMap, ptBins, 2, -0.3) << std::endl;
  
}