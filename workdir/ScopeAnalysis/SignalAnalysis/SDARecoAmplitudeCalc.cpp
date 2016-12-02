#include "./SDARecoAmplitudeCalc.h"
#include "../JPetRecoSignalTools/JPetRecoSignalTools.h"
#include "sstream"
#include <stdlib.h>
#include "../../../framework/JPetManager/JPetManager.h"

ClassImp(SDARecoAmplitudeCalc);

SDARecoAmplitudeCalc::SDARecoAmplitudeCalc(const char* name, const char* title,
                     const char* in_file_suffix, const char* out_file_suffix) :
  JPetCommonAnalysisModule(name, title, in_file_suffix, out_file_suffix)
{
  setVersion(MODULE_VERSION);
}

SDARecoAmplitudeCalc::~SDARecoAmplitudeCalc()
{
}

void SDARecoAmplitudeCalc::begin()
{
	INFO(
	Form("Starting ampltidue calculation")
	);
	fBadSignals =0;
}

void SDARecoAmplitudeCalc::exec()
{
  // first take signal from fEvent
  fReader->getEntry(fEvent);
  const JPetRecoSignal& signal = (JPetRecoSignal&) fReader->getData();

  //calculate charge of the signal
  double amp= JPetRecoSignalTools::calculateAmplitude(signal);

  //to save Reco signal one needs to copy it to non const variable
  JPetRecoSignal signalWithAmp = signal;

  //setting offset of signal
  signalWithAmp.setAmplitude(amp);
  
  //saving singal into output root file
  fWriter->write(signalWithAmp);

  // increase event counter
  fEvent++;

}

void SDARecoAmplitudeCalc::end()
{
	double goodPercent = (fEventNb-fBadSignals) * 100.0/fEventNb;
	INFO(
	Form("Amplitude calculation complete \nAmount of bad signals: %d \n %f %% of data is good" , fBadSignals, goodPercent) );
}
