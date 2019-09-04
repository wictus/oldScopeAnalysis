/**
 * @file SDAHitPositions.h
 * @author Szymon Niedzwiecki, szymon.niedzwiecki@googlemail.com
 * @copyright Copyright (c) 2015, Szymon Niedzwiecki
 * @brief Calculates hit positions using one threshold and mean velocity
 * Reads a TTree of Hits and fills them with time differences, hit positions and time of hits
 * Simplest approach with one threshold and mean velocity was used.
 */

#ifndef _JPETANALYSISMODULE_SDAHITPOSITIONS_H_
#define _JPETANALYSISMODULE_SDAHITPOSITIONS_H_

#include "../../../framework/JPetCommonAnalysisModule/JPetCommonAnalysisModule.h"
#include "TCanvas.h"
#ifdef MODULE_VERSION
#undef MODULE_VERSION
#endif

#define MODULE_VERSION 1

class SDAHitPositions: public JPetCommonAnalysisModule
{

public:

  SDAHitPositions(const char* name, const char* title,
                  const char* in_file_suffix, const char* out_file_suffix, 
                  const double threshold, const double velocity);
  virtual ~SDAHitPositions();
  virtual void exec();
  virtual void begin();
  virtual void end();

  ClassDef(SDAHitPositions, MODULE_VERSION );

private:
	double fGivenThreshold;
	double fGivenVelocity;
};

#endif
