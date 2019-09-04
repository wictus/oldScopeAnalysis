/**
 * @file SDAHitCcutOnEnergy.h
 * @author Szymon Niedzwiecki, szymon.niedzwiecki@googlemail.com
 * @copyright Copyright (c) 2015, Szymon Niedzwiecki
 * @brief Selector of events for high deposited energy
 * Reads a TTree of JPetHit, and checks if their energy is higher than given threshold in energy [keV]
 */

#ifndef _JPETANALYSISMODULE_SDAHITCUTONEENERGY_H_
#define _JPETANALYSISMODULE_SDAHITCUTONEENERGY_H_

#include "../../../framework/JPetCommonAnalysisModule/JPetCommonAnalysisModule.h"
#ifdef MODULE_VERSION
#undef MODULE_VERSION
#endif

#define MODULE_VERSION 1


class SDAHitCutOnEnergy : public JPetCommonAnalysisModule
{

public:

  SDAHitCutOnEnergy(const char* name, const char* title,
               const char* in_file_suffix, const char* out_file_suffix, const double thresholdValue);
  virtual ~SDAHitCutOnEnergy();
  virtual void exec();
  virtual void begin();
  virtual void end();
  ClassDef(SDAHitCutOnEnergy, MODULE_VERSION );

private:
  double fThresholdEnergy;
  int fAboveThreshold;
};

#endif


