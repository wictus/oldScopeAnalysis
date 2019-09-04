/**
 * @file SDAHitFindEnergyConstant.h
 * @author Szymon Niedzwiecki, szymon.niedzwiecki@googlemail.com
 * @copyright Copyright (c) 2015, Szymon Niedzwiecki
 * @brief Finder of energy calibration constant 
 * Reads a TTree of JPetHit, forms Qleft + Qright spectrum and fits to it MC spectrum
 * then saves spectrum width parameter as calibration constant for given scintillator
 */

#ifndef _JPETANALYSISMODULE_FINDENERGYCONSTANT_H_
#define _JPETANALYSISMODULE_FINDENERGYCONSTANT_H_

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


class SDAHitFindEnergyConstant : public JPetCommonAnalysisModule
{

public:

  SDAHitFindEnergyConstant(const char* name, const char* title,
               const char* in_file_suffix, const char* out_file_suffix);
  virtual ~SDAHitFindEnergyConstant();
  virtual void exec();
  virtual void begin();
  virtual void end();
  ClassDef(SDAHitFindEnergyConstant, MODULE_VERSION );

private:
  int fScintillatorID;
  std::vector<double> fEXPEvents;
  std::vector<double> fEnergyConstants;
  std::vector<double> fEnergyResolutions;
};

#endif

