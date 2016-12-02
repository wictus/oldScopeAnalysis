#ifndef _TOTVSQ_H
#define _TOTVSQ_H

#include "../../../framework/JPetCommonAnalysisModule/JPetCommonAnalysisModule.h"
#include <string>
#include <map>
#include <utility>
#include <vector>
#include <sstream>
#include "TGraph.h"
#include "TCanvas.h"

class TOTVSQ: public JPetCommonAnalysisModule
{
public:
  TOTVSQ(const char* name, const char* title, const char* in_file_suffix, const char* out_file_suffix, const std::string threshold);
  virtual ~TOTVSQ();
  virtual void begin();
  virtual void end();
  virtual void exec();

private:
  std::string fThreshold;
  std::map<int, std::vector<std::pair<double, double>>> fValuesPerPMT;
  
};

#endif //_TOTVSQ_H