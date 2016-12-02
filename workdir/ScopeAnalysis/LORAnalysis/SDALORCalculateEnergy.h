/**
 * @file SDALORCalculateEnergy.h
 * @author Szymon Niedzwiecki, szymon.niedzwiecki@googlemail.com
 * @copyright Copyright (c) 2015, Szymon Niedzwiecki
 * @brief Calculator of deposited energy
 * Reads a TTree of JPetLOR, forms Qleft + Qright / 2, multiplies it by scaling factor and saves as deposited energy
 */

#ifndef _JPETANALYSISMODULE_SDALORCALCULATEENERGY_H_
#define _JPETANALYSISMODULE_SDALORCALCULATEENERGY_H_

#include "../../../framework/JPetCommonAnalysisModule/JPetCommonAnalysisModule.h"
#include "utility"
#ifdef MODULE_VERSION
#undef MODULE_VERSION
#endif

#define MODULE_VERSION 1


class SDALORCalculateEnergy : public JPetCommonAnalysisModule
{

public:

  SDALORCalculateEnergy(const char* name, const char* title,
               const char* in_file_suffix, const char* out_file_suffix, std::vector< std::pair <int, double>> calibration );
  virtual ~SDALORCalculateEnergy();
  virtual void exec();
  virtual void begin();
  virtual void end();
  ClassDef(SDALORCalculateEnergy, MODULE_VERSION );

private:
  std::vector< std::pair <int,double>> fCalibration;
  double fEnergy, fEnergyFactor;
};

#endif


