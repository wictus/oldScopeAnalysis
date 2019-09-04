/**
*  @file SDADoTOFCalib.h
*  @author Szymon Niedźwiecki, szymon.niedzwiecki@googlemail.com
*  @brief estimator of TOF calibration constant
*  Reads file of JPetLORs and applies TOF calibration
*/

#ifndef _JPETANALYSISMODULE_SDADOTOFCALIB_H_
#define _JPETANALYSISMODULE_SDADOTOFCALIB_H_

#include "../../../framework/JPetCommonAnalysisModule/JPetCommonAnalysisModule.h"

class SDADoTOFCalib : public JPetCommonAnalysisModule
{

  public:
  SDADoTOFCalib(const char* name, const char* title,
               const char* in_file_suffix, const char* out_file_suffix, double calibration) ;
  virtual ~SDADoTOFCalib();
  virtual void exec();
  virtual void begin();
  virtual void end();

  private:
  double fCalibration;
};

#endif

