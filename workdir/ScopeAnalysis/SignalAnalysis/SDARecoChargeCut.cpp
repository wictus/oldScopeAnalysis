#include "./SDARecoChargeCut.h"
#include "../JPetRecoSignalTools/JPetRecoSignalTools.h"
#include "sstream"
#include <stdlib.h>
#include "../../../framework/JPetManager/JPetManager.h"

ClassImp(SDARecoChargeCut);

SDARecoChargeCut::SDARecoChargeCut(const char* name, const char* title,
                     const char* in_file_suffix, const char* out_file_suffix, std::pair<double, double> ampCuts) :
  JPetCommonAnalysisModule(name, title, in_file_suffix, out_file_suffix), fChargeCuts( ampCuts )
{
  setVersion(MODULE_VERSION);
}

SDARecoChargeCut::~SDARecoChargeCut()
{
}

void SDARecoChargeCut::begin()
{
	INFO(
	Form("Starting amplitude cuts from %f to %f mV", fAmpCuts.first, fAmpCuts.second)
	);	
}

void SDARecoChargeCut::exec()
{
  // first take signal from fEvent
  fReader->getEntry(fEvent);
  const JPetRecoSignal& signal = (JPetRecoSignal&) fReader->getData();

  //saving singal into output root file if it survives the cuts
  if ( signal.getPM().getScin().getID() == 1 )
  {
    if ( signal.getAmplitude() > fChargeCuts.first && signal.getAmplitude() < fChargeCuts.second )
      fWriter->write(signal);
  }
  else
    fWriter->write(signal);

  // increase event counter
  fEvent++;

}

void SDARecoChargeCut::end()
{
	INFO(
	Form("Charge cuts complete") 
	);
}
