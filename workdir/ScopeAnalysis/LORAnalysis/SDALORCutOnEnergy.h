/**
 * @file SDALORCcutOnEnergy.h
 * @author Szymon Niedzwiecki, szymon.niedzwiecki@googlemail.com
 * @copyright Copyright (c) 2015, Szymon Niedzwiecki
 * @brief Selector of events for high deposited energy
 * Reads a TTree of JPetLOR, and checks if their energy is higher than given threshold in energy [keV]
 */

#ifndef _JPETANALYSISMODULE_SDALORCUTONEENERGY_H_
#define _JPETANALYSISMODULE_SDALORCUTONEENERGY_H_

#include "../../../framework/JPetCommonAnalysisModule/JPetCommonAnalysisModule.h"
#ifdef MODULE_VERSION
#undef MODULE_VERSION
#endif

#define MODULE_VERSION 1


class SDALORCutOnEnergy : public JPetCommonAnalysisModule
{

public:

  SDALORCutOnEnergy(const char* name, const char* title,
               const char* in_file_suffix, const char* out_file_suffix, const double thresholdValue);
  virtual ~SDALORCutOnEnergy();
  virtual void exec();
  virtual void begin();
  virtual void end();
  ClassDef(SDALORCutOnEnergy, MODULE_VERSION );

private:
  double fThresholdEnergy;
  double fThresholdEnergyUpper;
  int fAboveThreshold;
};

#endif


