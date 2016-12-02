/**
 * @file SDAMatchHits.h
 * @author Szymon Niedzwiecki, szymon.niedzwiecki@googlemail.com
 * @copyright Copyright (c) 2015, Szymon Niedzwiecki
 * @brief Producer of JPetHit objects for SDA signals
 * Reads a TTree of PhysSignals matches the ones with the same TSlot 
 * and joins the ones from the same scintillator into JPetHit object
  */

#ifndef _JPETANALYSISMODULE_SDAMATCHHITS_H_
#define _JPETANALYSISMODULE_SDAMATCHHITS_H_

#include "../../../framework/JPetCommonAnalysisModule/JPetCommonAnalysisModule.h"
#include "TCanvas.h"
#ifdef MODULE_VERSION
#undef MODULE_VERSION
#endif

#define MODULE_VERSION 1

class SDAMatchHits: public JPetCommonAnalysisModule
{

public:

  SDAMatchHits(const char* name, const char* title,
               const char* in_file_suffix, const char* out_file_suffix);
  virtual ~SDAMatchHits();
  virtual void exec();
  virtual void begin();
  virtual void end();

  ClassDef(SDAMatchHits, MODULE_VERSION );

private:
  // put any custom variables (e.g. histograms) here:
	int fNotMatched;
	int fMatched;
	int fTSlot;
	std::vector<JPetPhysSignal> fArray;
	std::vector<std::vector<JPetPhysSignal> > fSegragatedSignals; // segragated by strip ID
};

#endif
