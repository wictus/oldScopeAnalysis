/**
 * @copyright Copyright (c) 2015, The J-Pet Framework Authors.
 * @file bossOutput.h
 * @brief outputs file in standard defined by P.M.
 * Writes down an ASCII file with data in form: time at thresholds, charges, energy scaling factors, energy resolution, energy deposited
 */

#ifndef _JPETANALYSISMODULE_BOSSOUTPUT_H_
#define _JPETANALYSISMODULE_BOSSOUTPUT_H_

#include "../../../framework/JPetCommonAnalysisModule/JPetCommonAnalysisModule.h"
#include "TCanvas.h"
#ifdef MODULE_VERSION
#undef MODULE_VERSION
#endif

#define MODULE_VERSION 1

class bossOutput: public JPetCommonAnalysisModule
{

public:

  bossOutput(const char* name, const char* title,
           const char* in_file_suffix, const char* out_file_suffix);
  virtual ~bossOutput();
  virtual void exec();
  virtual void begin();
  virtual void end();

  ClassDef(bossOutput, MODULE_VERSION );

private:
  std::ofstream outFile;
};

#endif // _JPETANALYSISMODULE_BOSSOUTPUT_H                          
