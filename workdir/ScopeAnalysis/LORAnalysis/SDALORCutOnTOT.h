/**
 * @file SDALORCutOnTOT.h
 * @author Szymon Niedzwiecki, szymon.niedzwiecki@googlemail.com
 * @copyright Copyright (c) 2015, Szymon Niedzwiecki
 * @brief Selector of events for high deposited energy
 * Reads a TTree of JPetLOR, and checks if TOT on one strip is higher than given threshold in TOT[ps]
 */

#ifndef _JPETANALYSISMODULE_SDALORCUTONTOT_H_
#define _JPETANALYSISMODULE_SDALORCUTONTOT_H_

#include "../../../framework/JPetCommonAnalysisModule/JPetCommonAnalysisModule.h"
#ifdef MODULE_VERSION
#undef MODULE_VERSION
#endif

#define MODULE_VERSION 1


class SDALORCutOnTOT : public JPetCommonAnalysisModule
{

public:

  SDALORCutOnTOT(const char* name, const char* title,
               const char* in_file_suffix, const char* out_file_suffix, 
	       const double thresholdValue, const int PMTID);
  virtual ~SDALORCutOnTOT();
  virtual void exec();
  virtual void begin();
  virtual void end();
  ClassDef(SDALORCutOnTOT, MODULE_VERSION );

private:
  void performCut(const JPetLOR& lor);
  JPetPhysSignal findSignalToCut(const JPetLOR& lor);
  double fTOTThreshold;
  int fPMTID;
  int fAboveThreshold;
};

#endif


