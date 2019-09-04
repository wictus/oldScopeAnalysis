/**
 * @file SDARecoFallingTimeCalc.h
 * @author Szymon Niedzwiecki, szymon.niedzwiecki@googlemail.com
 * @copyright Copyright (c) 2015, Szymon Niedzwiecki
 * @brief Calculates falling times for each JPetRecoSignal from SDA
 * Reads a TTree of JPetRecoSignals and calculates their falling time. 
 */

#ifndef _JPETANALYSISMODULE_SDARECOFALLINGTIMECALC_H_
#define _JPETANALYSISMODULE_SDARECOFALLINGTIMECALC_H_

#include "../../../framework/JPetCommonAnalysisModule/JPetCommonAnalysisModule.h"
#include "TCanvas.h"
#include <vector>
#include "TH1F.h"
#ifdef MODULE_VERSION
#undef MODULE_VERSION
#endif

#define MODULE_VERSION 1



class SDARecoFallingTimeCalc: public JPetCommonAnalysisModule
{

public:

  SDARecoFallingTimeCalc(const char* name, const char* title,
           const char* in_file_suffix, const char* out_file_suffix);
  virtual ~SDARecoFallingTimeCalc();
  virtual void exec();
  virtual void begin();
  virtual void end();

  ClassDef(SDARecoFallingTimeCalc, MODULE_VERSION );

private:
	std::vector<TH1F*> fFallHistos, fFallHistosRescaled;
	std::vector<std::vector<double> > fFallTimes;
        double fFall;
	std::vector<int> fIDs;
	unsigned int fNumberOfPMTs;
	std::string fFileName;
  // put any custom variables (e.g. histograms) here:
};

#endif
