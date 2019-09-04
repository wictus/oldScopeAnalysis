/**
* @file SDALORCalculateWalkCorrection.h
* @author Szymon Niedźwiecki
* @brief Producer of walk correction to measured times
* Reads a TTree of JPetLORs, plots deltaT vs 1/sqrt(E) and fits line, coefficients of the line can be accessed
*/

#ifndef _JPETANALYSISMODULE_SDALORCALCULATEWALKCORRECTION_H_
#define _JPETANALYSISMODULE_SDALORCALCULATEWALKCORRECTION_H_

#include "../../../framework/JPetCommonAnalysisModule/JPetCommonAnalysisModule.h"
#include <vector>
#include <fstream>
#include <map>
#include <sstream>
#include <utility>
#include <cmath>
#include "TCanvas.h"
#include "TH2F.h"
#include "TGraph.h"
#include "TF1.h"
#include "TStyle.h"
#include <TString.h>

#ifdef MODULE_VERSION
#undef MODULE_VERSION
#endif

#define MODULE_VERSION 1



struct walkCoefficients
{
  double alpha;
  double beta;
  int scintID;
};

class SDALORCalculateWalkCorrection : public JPetCommonAnalysisModule{

	public:
	  SDALORCalculateWalkCorrection( const char* name, const char* title, const char* in_file_suffix, const char* out_file_suffix, std::map<int, double> thresholds);
	  virtual ~SDALORCalculateWalkCorrection();
	  virtual void exec();
	  virtual void begin();
	  virtual void end();

	  ClassDef( SDALORCalculateWalkCorrection, MODULE_VERSION );

	private:
	  void prepareTimesUsingTOT(JPetLOR& fLOR);
	  void doWalkForTOF();
	  void doWalkForTimeDifference();
	  void doWalkCorrectionForEachPMTIndividually();
	  void prepareTimesForPMTCorrection(JPetLOR& fLOR);
	  void prepareTimesForScintCorrection(JPetLOR& fLOR);
	  std::map<int, double> fThresholds;
	  std::vector<TH2F*> walkPlots;
	  std::vector<walkCoefficients> coefficients; 
	  std::map<int, std::vector<std::pair<double,double> > > points; //for different strips
	  std::vector<std::pair<double,double> > fTOFs; //for all strips
};

#endif
