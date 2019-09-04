/**
 * @file twoDRecoOutput.h
 * @author Szymon Niedzwiecki, szymon.niedzwiecki@googlemail.com
 * @copyright Copyright (c) 2015, Szymon Niedzwiecki
 * @brief Calculates TOF for JPetLORs and outputs them into text file
 * Reads a TTree of JPetLOR, calculates their TOF and produces file for image reconstruction
 * In reconstruction file there are three collums: hit position top strip [cm],
 * hit position bottom strip [cm], TOF [ps]
 */

#ifndef _JPETANALYSISMODULE_TWODRECOOUTPUT_H_
#define _JPETANALYSISMODULE_TWODRECOOUTPUT_H_

#include "../../../framework/JPetCommonAnalysisModule/JPetCommonAnalysisModule.h"
#include "TCanvas.h"
#ifdef MODULE_VERSION
#undef MODULE_VERSION
#endif

#define MODULE_VERSION 1

class twoDRecoOutput: public JPetCommonAnalysisModule
{

public:

  twoDRecoOutput(const char* name, const char* title,
          const char* in_file_suffix, const char* out_file_suffix);
  virtual ~twoDRecoOutput();
  virtual void exec();
  virtual void begin();
  virtual void end();

  ClassDef(twoDRecoOutput, MODULE_VERSION );

private:
  std::vector<double> fZ1,fZ2, fTOF;
  std::ofstream outFile;
};

#endif

