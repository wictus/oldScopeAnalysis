/**
 * @file drawAllCharges.h
 * @author Szymon Niedzwiecki, szymon.niedzwiecki@googlemail.com
 * @copyright Copyright (c) 2015, Szymon Niedzwiecki
 * @brief Draws charges spectra for PMT
 * Reads a TTree of JPetRecoSignals and fills charge values from PMTs to the histo. 
 */

#ifndef _JPETANALYSISMODULE_DRAWALLCHARGES_H_
#define _JPETANALYSISMODULE_DRAWALLCHARGES_H_

#include "../../../framework/JPetCommonAnalysisModule/JPetCommonAnalysisModule.h"
#include "TCanvas.h"
#include "TH1F.h"
#include <sstream>
#ifdef MODULE_VERSION
#undef MODULE_VERSION
#endif

#define MODULE_VERSION 1

class drawAllCharges: public JPetCommonAnalysisModule
{

public:

  drawAllCharges(const char* name, const char* title,
           const char* in_file_suffix, const char* out_file_suffix);
  virtual ~drawAllCharges();
  virtual void exec();
  virtual void begin();
  virtual void end();

  ClassDef(drawAllCharges, MODULE_VERSION );

private:
  // put any custom variables (e.g. histograms) here:
	std::vector<TH1F*> fChargeHistos;
	std::vector<std::vector<double> > fCharges;
        double fCharge;
	std::vector<int> fIDs;
	unsigned int fNumberOfPMTs;
	std::string fFileName;
};

#endif

