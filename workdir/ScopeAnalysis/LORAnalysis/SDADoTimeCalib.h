/**
*  @file SDADoTimeCalib.h
*  @author Szymon Niedźwiecki, szymon.niedzwiecki@googlemail.com
*  @brief estimator of time difference calibration constants
*  Reads file of JPetLORs and constructs histograms of time difference for each strip
*  writes such histogram with gaus fitted into png file
*/

#ifndef _JPETANALYSISMODULE_SDADOTIMECALIB_H_
#define _JPETANALYSISMODULE_SDADOTIMECALIB_H_

#include "../../../framework/JPetCommonAnalysisModule/JPetCommonAnalysisModule.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TStyle.h"
#include "utility"

class SDADoTimeCalib : public JPetCommonAnalysisModule
{

  public:
  SDADoTimeCalib(const char* name, const char* title,
               const char* in_file_suffix, const char* out_file_suffix, std::vector< std::pair <int, double>> timeCalib) ;
  virtual ~SDADoTimeCalib();
  virtual void exec();
  virtual void begin();
  virtual void end();

  private:
  std::vector< std::pair <int,double>> fCalibration;
};

#endif
