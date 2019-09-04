/**
 * @file SDARecoFallingTimeCalc.h
 * @author Szymon Niedzwiecki, szymon.niedzwiecki@googlemail.com
 * @copyright Copyright (c) 2015, Szymon Niedzwiecki
 * @brief Calculates falling times for each JPetRecoSignal from SDA
 * Reads a TTree of JPetRecoSignals and calculates their falling time. 
 */

#ifndef _JPETANALYSISMODULE_SDARECOENERGYRESOLUTIONFIT_H_
#define _JPETANALYSISMODULE_SDARECOENERGYRESOLUTIONFIT_H_

#include "../../../framework/JPetCommonAnalysisModule/JPetCommonAnalysisModule.h"
#include "TCanvas.h"
#include <vector>
#include <fstream>
#include "TH1F.h"
#include "TF1.h"
#ifdef MODULE_VERSION
#undef MODULE_VERSION
#endif

#define MODULE_VERSION 1



class SDARecoEnergyResolutionFit: public JPetCommonAnalysisModule
{

public:

  SDARecoEnergyResolutionFit(const char* name, const char* title,
           const char* in_file_suffix, const char* out_file_suffix);
  virtual ~SDARecoEnergyResolutionFit();
  virtual void exec();
  virtual void begin();
  virtual void end();

  ClassDef(SDARecoEnergyResolutionFit, MODULE_VERSION );

private:
	std::vector<TH1F*> fChargeHistos, fChargeHistosRescaled;
	std::vector<std::vector<double> > fCharges;
        double fCharge;
	std::vector<int> fIDs;
	unsigned int fNumberOfPMTs;
	std::string fFileName;
  // put any custom variables (e.g. histograms) here:
};

#endif
