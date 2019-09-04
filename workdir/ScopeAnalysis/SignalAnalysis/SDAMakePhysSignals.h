/**
 * @file SDAMakePhysSignals.h
 * @author Szymon Niedzwiecki, szymon.niedzwiecki@googlemail.com
 * @copyright Copyright (c) 2015, Szymon Niedzwiecki
 * @brief Dummy producer of JPetPhysSignal objects
 * Reads a TTree of RecoSignals and transforms them into JPetPhysSignal objects
 * PhysSignals have charge value of their RecoSignals set as fPhe
 */

#ifndef _JPETANALYSISMODULE_SDAMAKEPHYSSIGNALS_H_
#define _JPETANALYSISMODULE_SDAMAKEPHYSSIGNALS_H_

#include "../../../framework/JPetCommonAnalysisModule/JPetCommonAnalysisModule.h"
#include "TCanvas.h"
#include <cmath>
#include <map>
#include "TF1.h"
#include <map>
#ifdef MODULE_VERSION
#undef MODULE_VERSION
#endif

#define MODULE_VERSION 1


class SDAMakePhysSignals: public JPetCommonAnalysisModule
{

public:

  SDAMakePhysSignals(const char* name, const char* title,
                     const char* in_file_suffix, const char* out_file_suffix,
		     std::map<int, TF1 > gainCurves, std::map<int, double> voltages
                     );
  virtual ~SDAMakePhysSignals();
  virtual void exec();
  virtual void begin();
  virtual void end();

  ClassDef(SDAMakePhysSignals, MODULE_VERSION );

private:
  std::map<int,TF1> fGainCurves;
  std::map<int, double> fVoltages;

};

#endif
