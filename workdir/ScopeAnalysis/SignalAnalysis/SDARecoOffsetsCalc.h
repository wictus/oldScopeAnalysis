/**
 * @file SDARecoOffsetsCalc.h
 * @author Szymon Niedzwiecki, szymon.niedzwiecki@googlemail.com
 * @copyright Copyright (c) 2015, Szymon Niedzwiecki
 * @brief Calculates offsets for each JPetRecoSignal from SDA
 * Reads a TTree of JPetRecoSignals and calculates is offset value. For more info
 * look into Monika Pawlik-Niedzwiecka master thesis. 
 */

#ifndef _JPETANALYSISMODULE_SDARECOOFFSETCALC_H_
#define _JPETANALYSISMODULE_SDARECOOFFSETCALC_H_

#include "../../../framework/JPetCommonAnalysisModule/JPetCommonAnalysisModule.h"
#include "TCanvas.h"
#ifdef MODULE_VERSION
#undef MODULE_VERSION
#endif

#define MODULE_VERSION 1

class SDARecoOffsetsCalc: public JPetCommonAnalysisModule
{

public:

  SDARecoOffsetsCalc(const char* name, const char* title,
           const char* in_file_suffix, const char* out_file_suffix);
  virtual ~SDARecoOffsetsCalc();
  virtual void exec();
  virtual void begin();
  virtual void end();

  ClassDef(SDARecoOffsetsCalc, MODULE_VERSION );

private:
  // put any custom variables (e.g. histograms) here:
	double fOffset;
	int fBadSignals;
};

#endif
