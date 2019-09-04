/**
 * @file drawOffset.h
 * @author Szymon Niedzwiecki, szymon.niedzwiecki@googlemail.com
 * @copyright Copyright (c) 2015, Szymon Niedzwiecki
 * @brief Draws offset spectrum for chosen PMT
 * Reads a TTree of JPetRecoSignals and fills offset values from selected PMT to the histo. 
 */

#ifndef _JPETANALYSISMODULE_DRAWOFFSET_H_
#define _JPETANALYSISMODULE_DRAWOFFSET_H_

#include "../../../framework/JPetCommonAnalysisModule/JPetCommonAnalysisModule.h"
#include "TCanvas.h"
#include "TH1F.h"
#include <sstream>
#ifdef MODULE_VERSION
#undef MODULE_VERSION
#endif

#define MODULE_VERSION 1

class drawOffset: public JPetCommonAnalysisModule
{

public:

  drawOffset(const char* name, const char* title,
           const char* in_file_suffix, const char* out_file_suffix, const int PMTiD);
  virtual ~drawOffset();
  virtual void exec();
  virtual void begin();
  virtual void end();

  ClassDef(drawOffset, MODULE_VERSION );

private:
  // put any custom variables (e.g. histograms) here:
	TH1F* fOffsetHisto;
	std::vector<double> fOffsets;
        double fOffset;
	int fID;
	bool fFoundPMTinParam;
};

#endif

