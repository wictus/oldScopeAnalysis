/**
 * @file SDALORExctractAmplitudes.h
 * @author Szymon Niedzwiecki, szymon.niedzwiecki@googlemail.com
 * @copyright Copyright (c) 2016, Szymon Niedzwiecki
 * @brief Finder of amplitude calibration constant 
 * Reads a TTree of JPetLOR, forms amplitude spectrum and fits to it MC spectrum
 * then saves spectrum width parameter as calibration constant for given photmultiplier
 */

#ifndef _JPETANALYSISMODULE_SDALOREXCTRACTAMPLITUDES_H_
#define _JPETANALYSISMODULE_SDALOREXCTRACTAMPLITUDES_H_

#include "../../../framework/JPetCommonAnalysisModule/JPetCommonAnalysisModule.h"
#include "../JPetHitTools/FindConstant.h"
#include "TH1F.h"
#include "TCanvas.h"
#include <vector>
#include <sstream>
#include <cmath>
#include "TRandom3.h"
#include "TGraph.h"
#include "TF1.h"
#include "TStyle.h"
#include <fstream>
#include "TUnixSystem.h"
#include <TMultiGraph.h>


#ifdef MODULE_VERSION
#undef MODULE_VERSION
#endif

#define MODULE_VERSION 1


class SDALORExctractAmplitudes: public JPetCommonAnalysisModule
{

public:

  SDALORExctractAmplitudes(const char* name, const char* title,
               const char* in_file_suffix, const char* out_file_suffix);
  virtual ~SDALORExctractAmplitudes();
  virtual void exec();
  virtual void begin();
  virtual void end();
  void saveToFile(std::string fileName, std::vector<double>& data);
  ClassDef(SDALORExctractAmplitudes, MODULE_VERSION );

private:
  int fPMTID ;
  std::vector<double> fEXPEvents;
  std::vector<double> fEnergyConstants;
  std::vector<double> fEnergyResolutions;
};

#endif

