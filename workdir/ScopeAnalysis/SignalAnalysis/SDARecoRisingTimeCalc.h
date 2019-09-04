/**
 * @file SDARecoRisingTimeCalc.h
 * @author Szymon Niedzwiecki, szymon.niedzwiecki@googlemail.com
 * @copyright Copyright (c) 2015, Szymon Niedzwiecki
 * @brief Calculates rising times for each JPetRecoSignal from SDA
 * Reads a TTree of JPetRecoSignals and calculates their rising time. 
 */

#ifndef _JPETANALYSISMODULE_SDARECORISINGTIMECALC_H_
#define _JPETANALYSISMODULE_SDARECORISINGTIMECALC_H_

#include "../../../framework/JPetCommonAnalysisModule/JPetCommonAnalysisModule.h"
#include "TCanvas.h"
#include <vector>
#include "TH1F.h"
#ifdef MODULE_VERSION
#undef MODULE_VERSION
#endif

#define MODULE_VERSION 1



class SDARecoRisingTimeCalc: public JPetCommonAnalysisModule
{

public:

  SDARecoRisingTimeCalc(const char* name, const char* title,
           const char* in_file_suffix, const char* out_file_suffix);
  virtual ~SDARecoRisingTimeCalc();
  virtual void exec();
  virtual void begin();
  virtual void end();

  ClassDef(SDARecoRisingTimeCalc, MODULE_VERSION );

private:
	std::vector<TH1F*> fRiseHistos, fRiseHistosRescaled;
	std::vector<std::vector<double> > fRiseTimes;
        double fRise;
	std::vector<int> fIDs;
	unsigned int fNumberOfPMTs;
	std::string fFileName;
  // put any custom variables (e.g. histograms) here:
};

#endif
