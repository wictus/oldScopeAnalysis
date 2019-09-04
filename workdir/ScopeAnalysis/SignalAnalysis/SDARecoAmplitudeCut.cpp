#include "./SDARecoAmplitudeCut.h"
#include "../JPetRecoSignalTools/JPetRecoSignalTools.h"
#include "sstream"
#include <stdlib.h>
#include "../../../framework/JPetManager/JPetManager.h"

ClassImp(SDARecoAmplitudeCut);

SDARecoAmplitudeCut::SDARecoAmplitudeCut(const char* name, const char* title,
                     const char* in_file_suffix, const char* out_file_suffix, std::pair<double, double> ampCuts) :
  JPetCommonAnalysisModule(name, title, in_file_suffix, out_file_suffix), fAmpCuts( ampCuts )
{
  setVersion(MODULE_VERSION);
}

SDARecoAmplitudeCut::~SDARecoAmplitudeCut()
{
}

void SDARecoAmplitudeCut::begin()
{
	INFO(
	Form("Starting amplitude cuts from %f to %f mV", fAmpCuts.first, fAmpCuts.second)
	);	
}

void SDARecoAmplitudeCut::exec()
{
  // first take signal from fEvent
  fReader->getEntry(fEvent);
  const JPetRecoSignal& signal = (JPetRecoSignal&) fReader->getData();

  //saving singal into output root file if it survives the cuts
  if ( signal.getPM().getScin().getID() == 1 )
  {
    if ( signal.getAmplitude() > fAmpCuts.first && signal.getAmplitude() < fAmpCuts.second )
      fWriter->write(signal);
  }
  else
    fWriter->write(signal);

  // increase event counter
  fEvent++;

}

void SDARecoAmplitudeCut::end()
{
	INFO(
	Form("Amplitude cuts complete") 
	);
}
