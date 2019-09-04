#ifndef _SDALORCALCULATENERGYFROMCALIBRATION_H
#define _SDALORCALCULATENERGYFROMCALIBRATION_H

#include "../../../framework/JPetCommonAnalysisModule/JPetCommonAnalysisModule.h"
#include <map>
#include <vector>
#include "TF1.h"

class SDALORCalculateEnergyFromCalibration : public JPetCommonAnalysisModule
{
public:
  SDALORCalculateEnergyFromCalibration(const char* name, const char* title, const char* in_file_suffix, const char* out_file_suffix, std::map<int, std::vector<double>> energyCalibration);
  virtual ~SDALORCalculateEnergyFromCalibration();
  virtual void begin();
  virtual void end();
  virtual void exec();
private:
  std::map<int, std::vector<double>> fCalibration;
  std::map<int, TF1*> fCalibrationFunctions;
  
};

#endif //_SDALORCALCULATENERGYFROMCALIBRATION_H