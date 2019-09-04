/**
 * @file SDARecoCutAroundPhotopeak.h
 * @author Szymon Niedzwiecki, szymon.niedzwiecki@googlemail.com
 * @copyright Copyright (c) 2019, Szymon Niedzwiecki
 * @brief Calculates falling times for each JPetRecoSignal from SDA
 * Reads a TTree of JPetRecoSignals and calculates their falling time. 
 */

#ifndef _JPETANALYSISMODULE_SDARECOCUTAROUNDPHOTOPEAK_H_
#define _JPETANALYSISMODULE_SDARECOCUTAROUNDPHOTOPEAK_H_

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



class SDARecoCutAroundPhotopeak: public JPetCommonAnalysisModule
{

public:

  SDARecoCutAroundPhotopeak(const char* name, const char* title,
           const char* in_file_suffix, const char* out_file_suffix, std::map<   std::string, std::pair < double, double > > photoPeak);
  virtual ~SDARecoCutAroundPhotopeak();
  virtual void exec();
  virtual void begin();
  virtual void end();

  ClassDef(SDARecoCutAroundPhotopeak, MODULE_VERSION );

private:
  std::map< std::string, std::pair < double, double > >  fChargeCuts;
  std::string fFileName;
  // put any custom variables (e.g. histograms) here:
};

#endif //SDARECOCUTAROUNDPHOTOPEAK
