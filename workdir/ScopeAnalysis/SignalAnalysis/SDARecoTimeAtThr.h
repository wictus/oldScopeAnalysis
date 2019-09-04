/**
 * @file SDARecoTimeAtThr.h
 * @author Szymon Niedzwiecki, szymon.niedzwiecki@googlemail.com
 * @copyright Copyright (c) 2015, Szymon Niedzwiecki
 * @brief Calculates times at constant threshold for RecoSignals
 * Reads a TTree of Reco Signals and calculates times at constant threshold
 * with linear interpolation between points. Offset of signals is taken into account.
 */

#ifndef _JPETANALYSISMODULE_SDARECOTIMEATTHR_H_
#define _JPETANALYSISMODULE_SDARECOTIMEATTHR_H_

#include "../../../framework/JPetCommonAnalysisModule/JPetCommonAnalysisModule.h"
#include "TCanvas.h"
#ifdef MODULE_VERSION
#undef MODULE_VERSION
#endif

#define MODULE_VERSION 1

class SDARecoTimeAtThr: public JPetCommonAnalysisModule
{

public:

  SDARecoTimeAtThr(const char* name, const char* title, const char* in_file_suffix, const char* out_file_suffix, const std::vector<double>& thr);
  virtual ~SDARecoTimeAtThr();
  virtual void exec();
  virtual void begin();
  virtual void end();
  ClassDef(SDARecoTimeAtThr, MODULE_VERSION );

private:
  // put any custom variables (e.g. histograms) here:
  std::vector<double>  fThresholds; //for storing threshold value
  int fSignalsBelowThreshold;
};

#endif
