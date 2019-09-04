/**
 * @file ModuleC1.h
 * @author Aleksander Gajos, aleksander.gajos@uj.edu.pl
 * @copyright Copyright (c) 2014, Aleksander Gajos
 * @brief Producer of JPetRecoSignal objects
 * Reads a TTree of Raw Signals and transforms them into JPetRecoSignal objects
 * Reconstruction of signal shape, amplitude, offset, delay and charge can take place here.
 */

#ifndef _JPETANALYSISMODULE_SDAHITPOSITIONSRECAL_H_
#define _JPETANALYSISMODULE_SDAHITPOSITIONSRECAL_H_

#include "../../../framework/JPetCommonAnalysisModule/JPetCommonAnalysisModule.h"
#include "TCanvas.h"
#ifdef MODULE_VERSION
#undef MODULE_VERSION
#endif

#define MODULE_VERSION 1

class SDAHitPositionsReCal: public JPetCommonAnalysisModule
{

public:

  SDAHitPositionsReCal(const char* name, const char* title,
                       const char* in_file_suffix, const char* out_file_suffix, double recalTop, double recalBottom);
  virtual ~SDAHitPositionsReCal();
  virtual void exec();
  virtual void begin();
  virtual void end();

  ClassDef(SDAHitPositionsReCal, MODULE_VERSION );

private:
  // put any custom variables (e.g. histograms) here:
  double fReTop ;
  double fReBot ;
};

#endif
