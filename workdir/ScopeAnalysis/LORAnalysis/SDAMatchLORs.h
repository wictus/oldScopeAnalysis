/**
 * @file SDAMatchLORs.h
 * @author Szymon Niedzwiecki, szymon.niedzwiecki@googlemail.com
 * @copyright Copyright (c) 2015, Szymon Niedzwiecki
 * @brief Producer of JPetLOR 
 * Reads a TTree of JPetHit and transforms them into JPetLOR objects
 */

#ifndef _JPETANALYSISMODULE_SDAMATCHLORS_H_
#define _JPETANALYSISMODULE_SDAMATCHLORS_H_

#include "../../../framework/JPetCommonAnalysisModule/JPetCommonAnalysisModule.h"
#include "TCanvas.h"
#ifdef MODULE_VERSION
#undef MODULE_VERSION
#endif

#define MODULE_VERSION 1

class SDAMatchLORs: public JPetCommonAnalysisModule
{

public:

  SDAMatchLORs(const char* name, const char* title,
               const char* in_file_suffix, const char* out_file_suffix);
  virtual ~SDAMatchLORs();
  virtual void exec();
  virtual void begin();
  virtual void end();

  ClassDef(SDAMatchLORs, MODULE_VERSION );

private:
  std::vector<JPetHit> fArray;
  int fTSlot;
  int fNotMatched;
  int fMatched;
};

#endif
