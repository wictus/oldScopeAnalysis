/**
 * @file addCalibration.h
 * @author Szymon Niedźwiecki, szymon.niedzwiecki@googlemail.com
 * @copyright Copyright (c) 2015, Szymon Niedźwiecki
 * @brief Adder of calibration constants to PMTs in paramBank
 * Reads a root file and adds calibration constants to selected PMT in paramBank.
 * For more details look into README
 */

#ifndef _JPETANALYSISMODULE_ADDCALIBRATION_H_
#define _JPETANALYSISMODULE_ADDCALIBRATION_H_

#include "../../../framework/JPetCommonAnalysisModule/JPetCommonAnalysisModule.h"
#include "TCanvas.h"
#ifdef MODULE_VERSION
#undef MODULE_VERSION
#endif

#define MODULE_VERSION 1

class addCalibration: public JPetCommonAnalysisModule
{

public:

  addCalibration(const char* name, const char* title,
            const char* in_file_suffix, const char* out_file_suffix, const int PMTiD, const double p0, const double p1);
  virtual ~addCalibration();
  virtual void exec();
  virtual void begin();
  virtual void end();

  ClassDef(addCalibration, MODULE_VERSION );

private:
  int fID;
  double fGain0;
  double fGain1;
};

#endif

