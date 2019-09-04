/**
 * @file makeMeanSignal.h
 * @author Szymon Niedzwiecki, szymon.niedzwiecki@googlemail.com
 * @copyright Copyright (c) 2019, Szymon Niedzwiecki
 * @brief Draws charges spectra for PMT
 * Reads a TTree of JPetRecoSignals and fills charge values from PMTs to the histo. 
 */

#ifndef _JPETANALYSISMODULE_MAKEMEANSIGNAL_H_
#define _JPETANALYSISMODULE_MAKEMEANSIGNAL_H_

#include "../../../framework/JPetCommonAnalysisModule/JPetCommonAnalysisModule.h"
#include "TCanvas.h"
#include "TH1F.h"
#include <sstream>
#ifdef MODULE_VERSION
#undef MODULE_VERSION
#endif

#define MODULE_VERSION 1

class makeMeanSignal: public JPetCommonAnalysisModule
{

public:

  makeMeanSignal(const char* name, const char* title,
           const char* in_file_suffix, const char* out_file_suffix, int PMTID);
  virtual ~makeMeanSignal();
  virtual void exec();
  virtual void begin();
  virtual void end();

  ClassDef(makeMeanSignal, MODULE_VERSION );

private:
  std::vector<  JPetRecoSignal > signals;
  int fPMTID = 0;
  // put any custom variables (e.g. histograms) here:

};

#endif

