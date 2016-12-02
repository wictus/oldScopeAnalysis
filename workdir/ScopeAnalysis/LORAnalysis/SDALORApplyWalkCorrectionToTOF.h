#ifndef _SDAAPPLYWALKCORRECTIONTOTOF_H_
#define _SDAAPPLYWALKCORRECTIONTOTOF_H_

#include "../../../framework/JPetCommonAnalysisModule/JPetCommonAnalysisModule.h"
#include <cmath>
#include <sstream>
#include "TCanvas.h"
#include "TH1F.h"
#include "TStyle.h"


class SDALORApplyWalkCorrectionToTOF: public JPetCommonAnalysisModule{
public:
  SDALORApplyWalkCorrectionToTOF(const char* name, const char* title, const char* in_file_suffix, const char* out_file_suffix, const double coefficient);
  virtual ~SDALORApplyWalkCorrectionToTOF();
  virtual void exec();
  virtual void end();
  virtual void begin();
  
private:
  const double fCoefficient;
  std::vector<double> fTOF;
};


#endif //_SDAAPPLYWALKCORRECTIONTOTOF_H_