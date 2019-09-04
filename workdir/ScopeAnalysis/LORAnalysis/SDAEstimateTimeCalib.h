/**
*  @file SDAEstimateTimeCalib.h
*  @author Szymon Niedźwiecki, szymon.niedzwiecki@googlemail.com
*  @brief estimator of time difference calibration constants
*  Reads file if JPetLORs and constructs histograms of time difference for each strip
*  writes such histogram with gaus fitted into png file
*/

#ifndef _JPETANALYSISMODULE_SDAESTIMATETIMECALIB_H_
#define _JPETANALYSISMODULE_SDAESTIMATETIMECALIB_H_

#include "../../../framework/JPetCommonAnalysisModule/JPetCommonAnalysisModule.h"
#include <map>
#include "TCanvas.h"
#include "TH1F.h"
#include "TStyle.h"
#include <sstream>


class SDAEstimateTimeCalib : public JPetCommonAnalysisModule
{

  public:
  SDAEstimateTimeCalib(const char* name, const char* title,
               const char* in_file_suffix, const char* out_file_suffix, std::map<int, double> thresholds);
  virtual ~SDAEstimateTimeCalib();
  virtual void exec();
  virtual void begin();
  virtual void end();

  private:
  std::map<int, double> fSelectedThreshold;
  std::map<int, std::vector <double> > deltaTPerScint;

};

#endif
