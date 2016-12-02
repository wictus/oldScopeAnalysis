/**
 * @file SDAHitCalculateEnergy.h
 * @author Szymon Niedzwiecki, szymon.niedzwiecki@googlemail.com
 * @copyright Copyright (c) 2015, Szymon Niedzwiecki
 * @brief Calculator of deposited energy
 * Reads a TTree of JPetHit, forms Qleft + Qright / 2, multiplies it by scaling factor and saves as deposited energy
 */

#ifndef _JPETANALYSISMODULE_SDAHITCALCULATEENERGY_H_
#define _JPETANALYSISMODULE_SDAHITCALCULATEENERGY_H_

#include "../../../framework/JPetCommonAnalysisModule/JPetCommonAnalysisModule.h"
#ifdef MODULE_VERSION
#undef MODULE_VERSION
#endif

#define MODULE_VERSION 1


class SDAHitCalculateEnergy : public JPetCommonAnalysisModule
{

public:

  SDAHitCalculateEnergy(const char* name, const char* title,
               const char* in_file_suffix, const char* out_file_suffix);
  virtual ~SDAHitCalculateEnergy();
  virtual void exec();
  virtual void begin();
  virtual void end();
  ClassDef(SDAHitCalculateEnergy, MODULE_VERSION );

private:
  double fEnergy, fEnergyFactor;
};

#endif


