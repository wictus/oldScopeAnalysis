/**
 * @file SDARecoAmplitudeCalc.h
 * @author Szymon Niedźwiecki, szymon.niedzwiecki@googlemail.com
 * @copyright Copyright (c) 2015, Szymon Niedźwiecki
 * @brief Producer of charge for JPetRecoSignals
 * Reads a TTree of Reco Signals and calculates charges for each of them.
 * For more details look into README
 */

#ifndef _JPETANALYSISMODULE_SDAAMPLITUDE_H_
#define _JPETANALYSISMODULE_SDAAMPLITUDE_H_

#include "../../../framework/JPetCommonAnalysisModule/JPetCommonAnalysisModule.h"
#include "TCanvas.h"
#ifdef MODULE_VERSION
#undef MODULE_VERSION
#endif

#define MODULE_VERSION 1

class SDARecoAmplitudeCalc: public JPetCommonAnalysisModule
{

public:

  SDARecoAmplitudeCalc(const char* name, const char* title,
            const char* in_file_suffix, const char* out_file_suffix);
  virtual ~SDARecoAmplitudeCalc();
  virtual void exec();
  virtual void begin();
  virtual void end();

  ClassDef(SDARecoAmplitudeCalc, MODULE_VERSION );

private:
	int fBadSignals;
};

#endif

