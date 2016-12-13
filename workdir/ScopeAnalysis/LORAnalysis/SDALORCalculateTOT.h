#ifndef _JPETANALYSISMODULE_SDALORCALCULATETOT_H_
#define _JPETANALYSISMODULE_SDALORCALCULATETOT_H_

#include "../../../framework/JPetCommonAnalysisModule/JPetCommonAnalysisModule.h"

class SDALORCalculateTOT: public JPetCommonAnalysisModule
{
public:
  SDALORCalculateTOT(const char* name, const char* title, const char* in_file_suffix, const char* out_file_suffix, const double threshold);
  virtual ~SDALORCalculateTOT();
  virtual void begin();
  virtual void end();
  virtual void exec();
  ClassDef(SDALORCalculateTOT, 1 );
  
private:
  double fThreshold;
};

#endif //_SDALORCALCULATETOT_H_
