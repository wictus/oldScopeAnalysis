#include "./addCalibration.h"
#include "../JPetRecoSignalTools/JPetRecoSignalTools.h"
#include "sstream"
#include <stdlib.h>
#include "../../../framework/JPetManager/JPetManager.h"

ClassImp(addCalibration);

addCalibration::addCalibration(const char* name, const char* title,
                     const char* in_file_suffix, const char* out_file_suffix, const int PMTfID, 
                     const double p0, const double p1) :
  JPetCommonAnalysisModule(name, title, in_file_suffix, out_file_suffix)
{
  setVersion(MODULE_VERSION);
  fID = PMTfID;
  fGain0 = p0;
  fGain1 = p1;
}

addCalibration::~addCalibration()
{
}

void addCalibration::begin()
{
	INFO(
	Form("Saving calibration constants")
	);
   const JPetParamBank& paramBank = getParamBank();
   
   for( int i = 0; i < paramBank.getPMsSize(); i++)
   {
       if(paramBank.getPM(i).getID() == fID)
       {
           paramBank.getPM(i).setHVgain(fGain0,fGain1);
           INFO( Form ("Calibration added successfully") );
           break;
       }
   }
}

void addCalibration::exec()
{
}

void addCalibration::end()
{
	INFO(
	Form("Added calibration constants")
	);
}
