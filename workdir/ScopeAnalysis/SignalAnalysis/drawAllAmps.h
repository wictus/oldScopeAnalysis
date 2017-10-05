/**
 * @file drawAllAmps.h
 * @author Szymon Niedzwiecki, szymon.niedzwiecki@googlemail.com
 * @copyright Copyright (c) 2015, Szymon Niedzwiecki
 * @brief Draws amplitude spectra for PMTs
 * Reads a TTree of JPetRecoSignals and fills amplitude values from PMTs to the histo. 
 */

#ifndef _JPETANALYSISMODULE_DRAWALLAMPS_H_
#define _JPETANALYSISMODULE_DRAWALLAMPS_H_

#include "../../../framework/JPetCommonAnalysisModule/JPetCommonAnalysisModule.h"
#include "TCanvas.h"
#include "TH1F.h"
#include <sstream>
#ifdef MODULE_VERSION
#undef MODULE_VERSION
#endif

#define MODULE_VERSION 1

class drawAllAmps: public JPetCommonAnalysisModule
{

public:

  drawAllAmps(const char* name, const char* title,
           const char* in_file_suffix, const char* out_file_suffix);
  virtual ~drawAllAmps();
  virtual void exec();
  virtual void begin();
  virtual void end();

  ClassDef(drawAllAmps, MODULE_VERSION );

private:
  // put any custom variables (e.g. histograms) here:
	std::vector<TH1F*> fAmpHistos;
	std::vector<std::vector<double> > fAmps;
        double fAmp;
	std::vector<int> fIDs;
	unsigned int fNumberOfPMTs;
	std::string fFileName;
};

#endif

