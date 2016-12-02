#ifndef FINDCONSTANT_H
#define FINDCONSTANT_H
#include "../../../framework/JPetCommonAnalysisModule/JPetCommonAnalysisModule.h"
#include <sstream>
#include <cmath>
#include <fstream>
#include <algorithm>
#include "TRandom3.h"
#include "TGraph.h"
#include "TF1.h"
#include "TStyle.h"
#include "TUnixSystem.h"
#include "TMultiGraph.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TError.h"

#ifdef MODULE_VERSION
#undef MODULE_VERSION
#endif

#define MODULE_VERSION 1


class FindConstant{
	public:
  	  FindConstant(const std::vector<double>& inputEvents, const TString file, const double sourcePos, const int scinID);
	  double execute();	
	  double returnEnergyResolution();

	private:
	  void saveErrorsResults(const std::string& name);
	  double findLowResolutionError(const double eRes);
	  double findHighResolutionError(const double eRes);
	  void doFinalFit(const double eRes);
	  double drawChi2AndFitPol2(const std::vector<double>& res, const std::vector<double>& chi2);
	  void aproximateParameters();
	  void saveFitResultToTxt(std::string name);
	  void produceSIMEvents(std::vector<double>& SIMEvents, const double eRes);
	  void fillSIMHisto(const double normalisation = 1.0, const double alpha = 1.0);
	  void fillEXPHisto(const double normalisation = 1.0, const double alpha = 1.0);
	  double compareHistogramsByChi2(const double normalisation, const double alpha);
	  void saveFittedHisto(const double energyResolution);
	  void saveSIMHisto();
	  void saveEXPHisto();
	  void performFit(const double resolution);
	  bool isDir(std::string& path);
	  double estimateWidthRatio();
	  double estimateHeightRatio();
	  double findBestNorm(const double alpha);
	  double findBestAlpha(const double resolution);
	  double findBestBeta(const double start, const double stop, const double step, bool useFit);
	  std::vector< std::pair<double,double> > extractPointValuesAroundMinimum(const std::vector< std::pair<double,double>>& vec);
		
	   TString fileName;
	   TF1* quadraticFit;
	   double initialHeightRatio, initialWidthRatio, heighestCountsInSIM, heighestCountsInEXP;
	   double lowErrorEnergyResolution, highErrorEnergyResolution;
	   std::vector<double> SIMEvents, EXPEvents;
	   unsigned int numberOfBins;
	   int scintillatorID, binNumber;
	   TH1F* EXPHisto;
	   TH1F* SIMHisto;
	   std::string expHistoTitle;
	   double energyResolution, sourcePosition, initialNorm, initialAlpha;
	   int maxBin, minBin;
	   double bestChi2, bestNorm, bestAlpha, bestNumberOfBins;
	   TString filePath;
	   std::vector<double> alphasForStripes;
	   double upperCut, lowerCut;
	};
#endif

