#include "./SDARecoChargeCalc.h"
#include "../JPetRecoSignalTools/JPetRecoSignalTools.h"
#include "sstream"
#include <stdlib.h>
#include "../../../framework/JPetManager/JPetManager.h"

ClassImp(SDARecoChargeCalc);

SDARecoChargeCalc::SDARecoChargeCalc(const char* name, const char* title,
                     const char* in_file_suffix, const char* out_file_suffix) :
  JPetCommonAnalysisModule(name, title, in_file_suffix, out_file_suffix)
{
  setVersion(MODULE_VERSION);
}

SDARecoChargeCalc::~SDARecoChargeCalc()
{
}

void SDARecoChargeCalc::begin()
{
	INFO(
	Form("Starting charge calculation")
	);
	fBadSignals =0;
}

void SDARecoChargeCalc::exec()
{
  // first take signal from fEvent
  fReader->getEntry(fEvent);
  const JPetRecoSignal& signal = (JPetRecoSignal&) fReader->getData();

  //calculate charge of the signal
  double charge = JPetRecoSignalTools::calculateAreaFromStartingIndex(signal);

  // if something went wrong with calculation charge value will be equal to 999999
  if (charge == JPetRecoSignalTools::ERRORS::badCharge) {
	  WARNING( Form("Something went wrong when calculating charge for event: %d",fEvent)  );
	  JPetRecoSignalTools::saveBadSignalIntoRootFile(signal, fBadSignals, "badCharges.root");
	  fBadSignals++;
    fEvent++;
    return;
  }



  //to save Reco signal one needs to copy it to non const variable
  JPetRecoSignal signalWithCharge = signal;

  //setting offset of signal
  signalWithCharge.setCharge(charge);
  
  //saving singal into output root file
  fWriter->write(signalWithCharge);

  // increase event counter
  fEvent++;

}

void SDARecoChargeCalc::end()
{
	double goodPercent = (fEventNb-fBadSignals) * 100.0/fEventNb;
	INFO(
	Form("Charge` calculation complete \nAmount of bad signals: %d \n %f %% of data is good" , fBadSignals, goodPercent) );
}
