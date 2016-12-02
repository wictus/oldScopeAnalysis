/**
 * @copyright Copyright (c) 2015, The J-Pet Framework Authors.
 * @file thresholdScan.h
 * @brief outputs file in standard defined by P.M.
 * Writes down TGraph with time resolution at selected thresholds as a function of thresholds
 */

#ifndef _JPETANALYSISMODULE_THRESHOLDSCAN_H_
#define _JPETANALYSISMODULE_THRESHOLDSCAN_H_

#include "sstream"
#include "fstream"
#include "../../../framework/JPetCommonAnalysisModule/JPetCommonAnalysisModule.h"
#include "../JPetRecoSignalTools/JPetRecoSignalTools.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TF1.h"
#include "TStyle.h"
#include "TUnixSystem.h"
#include "TGraph.h"


#ifdef MODULE_VERSION
#undef MODULE_VERSION
#endif

#define MODULE_VERSION 1

class thresholdScan: public JPetCommonAnalysisModule
{

public:

  thresholdScan(const char* name, const char* title,
           const char* in_file_suffix, const char* out_file_suffix, const std::vector<double>& thresholds, const std::vector<double>& eCut);
  virtual ~thresholdScan();
  virtual void exec();
  virtual void begin();
  virtual void end();
  void plotHistosAndGraphs( std::vector<std::vector <double> >& data, std::string name);
  void plotEnergyCuts( std::vector<double>& data, std::string name);
  ClassDef(thresholdScan, MODULE_VERSION );

private:
  std::vector<double> energyCuts;
  std::vector<double> fThresholds;
  std::vector<double> energyTop, energyBottom;
  std::vector< std::vector<double> > fDeltaTimesForThresholdsTop;
  std::vector< std::vector<double> > fDeltaTimesForThresholdsBottom;

};

#endif // _JPETANALYSISMODULE_THRESHOLDSCAN_H                          
