/**
 * @file SDARecoChargeCalc.h
 * @author Szymon Niedźwiecki, szymon.niedzwiecki@googlemail.com
 * @copyright Copyright (c) 2015, Szymon Niedźwiecki
 * @brief Producer of charge for JPetRecoSignals
 * Reads a TTree of Reco Signals and calculates charges for each of them.
 * For more details look into README
 */

#ifndef _JPETANALYSISMODULE_SDACHARGE_H_
#define _JPETANALYSISMODULE_SDACHARGE_H_

#include "../../../framework/JPetCommonAnalysisModule/JPetCommonAnalysisModule.h"
#include "TCanvas.h"
#ifdef MODULE_VERSION
#undef MODULE_VERSION
#endif

#define MODULE_VERSION 1

class SDARecoChargeCalc: public JPetCommonAnalysisModule
{

public:

  SDARecoChargeCalc(const char* name, const char* title,
            const char* in_file_suffix, const char* out_file_suffix);
  virtual ~SDARecoChargeCalc();
  virtual void exec();
  virtual void begin();
  virtual void end();

  ClassDef(SDARecoChargeCalc, MODULE_VERSION );

private:
	int fBadSignals;
};

#endif

