/**
 * @copyright Copyright (c) 2015, The J-Pet Framework Authors.
 * @file bpOutput.h
 * @brief outputs file in standard defined by P.B.
 * Writes down an ASCII file with data in form: time at thresholds, charges, energy scaling factors, energy resolution, energy deposited
 */

#ifndef _JPETANALYSISMODULE_BPOUTPUT_H_
#define _JPETANALYSISMODULE_BPOUTPUT_H_

#include "../../../framework/JPetCommonAnalysisModule/JPetCommonAnalysisModule.h"
#include "TCanvas.h"
#ifdef MODULE_VERSION
#undef MODULE_VERSION
#endif

#define MODULE_VERSION 1

class bpOutput: public JPetCommonAnalysisModule
{

public:

  bpOutput(const char* name, const char* title,
           const char* in_file_suffix, const char* out_file_suffix);
  virtual ~bpOutput();
  virtual void exec();
  virtual void begin();
  virtual void end();

  ClassDef(bpOutput, MODULE_VERSION );

private:
  std::ofstream outFile;
};

#endif // _JPETANALYSISMODULE_BPOUTPUT_H 
