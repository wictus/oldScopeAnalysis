/**
*  @file SDACalculateTimes.h
*  @author Szymon Niedźwiecki, szymon.niedzwiecki@googlemail.com
*  @brief estimator of time difference calibration constants
*  Reads file of JPetLORs and constructs histograms of time difference for each strip
*  writes such histogram with gaus fitted into png file
*/

#ifndef _JPETANALYSISMODULE_SDACALCULATETIMES_H_
#define _JPETANALYSISMODULE_SDACALCULATETIMES_H_

#include <utility>
#include <map>
#include "../../../framework/JPetCommonAnalysisModule/JPetCommonAnalysisModule.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TStyle.h"

class SDACalculateTimes : public JPetCommonAnalysisModule
{

  public:
  SDACalculateTimes(const char* name, const char* title, const char* in_file_suffix, const char* out_file_suffix, std::map<int, double> thresholds) ;
  virtual ~SDACalculateTimes();
  virtual void exec();
  virtual void begin();
  virtual void end();

  private:
  std::map<int, double> fSelectedThreshold;
};

#endif

