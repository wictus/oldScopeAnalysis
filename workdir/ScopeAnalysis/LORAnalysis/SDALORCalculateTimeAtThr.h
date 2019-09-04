/**
* @file SDALORCalculateTimeAtThr.h
* @author Szymon Niedźwiecki
* @brief Producer of time at thresholds for JPetRecoSignals inside JPetLORs
* Reads a TTree of JPetLORs, extracts JPetRecoSignals from them and calculates time at given thresholds as would constant threshold discriminator do
*/

#ifndef _JPETANALYSISMODULE_SDALORCALCULATETIMEATTHR_H_
#define _JPETANALYSISMODULE_SDALORCALCULATETIMEATTHR_H_

#include "../../../framework/JPetCommonAnalysisModule/JPetCommonAnalysisModule.h"
#include <vector>

#ifdef MODULE_VERSION
#undef MODULE_VERSION
#endif

#define MODULE_VERSION 1

class SDALORCalculateTimeAtThr : public JPetCommonAnalysisModule{

	public:
	  SDALORCalculateTimeAtThr( const char* name, const char* title, const char* in_file_suffix, const char* out_file_suffix, const std::vector<double>& givenThresholds );
	  virtual ~SDALORCalculateTimeAtThr();
	  virtual void exec();
	  virtual void begin();
	  virtual void end();

	  ClassDef( SDALORCalculateTimeAtThr, MODULE_VERSION );

	private:
	  std::vector<double> fThresholds;
	  JPetLOR fCurrentLOR;
};

#endif
