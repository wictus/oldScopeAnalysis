/**
*  @file SDACalculatePositions.h
*  @author Szymon Niedźwiecki, szymon.niedzwiecki@googlemail.com
*  @brief estimator of time difference calibration constants
*  Reads file of JPetLORs and constructs histograms of time difference for each strip
*  writes such histogram with gaus fitted into png file
*/

#ifndef _JPETANALYSISMODULE_SDACALCULATEPOSITIONS_H_
#define _JPETANALYSISMODULE_SDACALCULATEPOSITIONS_H_

#include "../../../framework/JPetCommonAnalysisModule/JPetCommonAnalysisModule.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TStyle.h"
#include "utility"

class SDACalculatePositions : public JPetCommonAnalysisModule
{

  public:
  SDACalculatePositions(const char* name, const char* title,
              const char* in_file_suffix, const char* out_file_suffix,
	      const double threshold, const double positionCut) ;
  virtual ~SDACalculatePositions();
  virtual void exec();
  virtual void begin();
  virtual void end();

  private:
  double fSelectedThreshold;
  double fCut;
};

#endif

