/**
*  @file SDAEstimateTOFCalib.h
*  @author Szymon Niedźwiecki, szymon.niedzwiecki@googlemail.com
*  @brief estimator of time difference calibration constants
*  Reads file if JPetLORs and constructs histograms of TOF
*  writes such histogram with gaus fitted into png file
*/

#ifndef _JPETANALYSISMODULE_SDAESTIMATETOFCALIB_H_
#define _JPETANALYSISMODULE_SDAESTIMATETOFCALIB_H_

#include "../../../framework/JPetCommonAnalysisModule/JPetCommonAnalysisModule.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TStyle.h"
#include "sstream"

class SDAEstimateTOFCalib : public JPetCommonAnalysisModule
{

  public:
  SDAEstimateTOFCalib(const char* name, const char* title,
               const char* in_file_suffix, const char* out_file_suffix, const double threshold);
  virtual ~SDAEstimateTOFCalib();
  virtual void exec();
  virtual void begin();
  virtual void end();

  private:
  double fSelectedThreshold;
  std::vector<double> fTOFs;

};

#endif
