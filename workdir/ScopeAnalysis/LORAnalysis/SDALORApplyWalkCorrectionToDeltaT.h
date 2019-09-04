#ifndef _SDAAPPLYWALKCORRECTIONTODELTAT_H_
#define _SDAAPPLYWALKCORRECTIONTODELTAT_H_

#include "../../../framework/JPetCommonAnalysisModule/JPetCommonAnalysisModule.h"
#include <cmath>
#include <sstream>
#include "TCanvas.h"
#include "TH1F.h"
#include "TStyle.h"
#include <map>

class SDALORApplyWalkCorrectionToDeltaT: public JPetCommonAnalysisModule{
public:
  SDALORApplyWalkCorrectionToDeltaT(const char* name, const char* title, const char* in_file_suffix, const char* out_file_suffix, std::map<int, double> coefficient);
  virtual ~SDALORApplyWalkCorrectionToDeltaT();
  virtual void exec();
  virtual void end();
  virtual void begin();
  
private:
  std::map<int, double> fCoefficient;
};


#endif //_SDAAPPLYWALKCORRECTIONTODELTAT_H_