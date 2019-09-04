#include "./SDARecoTimeAtThr.h"
#include "../JPetRecoSignalTools/JPetRecoSignalTools.h"
#include "sstream"
#include <stdlib.h>

ClassImp(SDARecoTimeAtThr);

SDARecoTimeAtThr::SDARecoTimeAtThr(const char* name, const char* title,
                           const char* in_file_suffix, const char* out_file_suffix, const std::vector<double>& thr) :
  JPetCommonAnalysisModule(name, title, in_file_suffix, out_file_suffix)
{
  setVersion(MODULE_VERSION);
  fThresholds = thr;
}

SDARecoTimeAtThr::~SDARecoTimeAtThr()
{
}

void SDARecoTimeAtThr::begin()
{
	INFO(
      Form("Amount of signals in file: %d", fEventNb ) );
      fSignalsBelowThreshold = 0;
}

void SDARecoTimeAtThr::exec()
{

  // first take signal from fEvent
  fReader->getEntry(fEvent);
  const JPetRecoSignal signal = (JPetRecoSignal&) fReader->getData();

  //to save Reco signal one needs to copy it to non const variable
  JPetRecoSignal signalWithOffset = signal;
  //Calculate time at threshold
  for(unsigned int i = 0; i < fThresholds.size(); i++)
  {
	  double timeAtThr = JPetRecoSignalTools::calculateTimeAtThreshold(signal, fThresholds[i]);
	    //Checking if times crossed threshold, if not its value is set to 999999
	  if (timeAtThr == JPetRecoSignalTools::ERRORS::badTimeAtThr) {
        	INFO( Form("Signal has smaller amplitude than given threshold: %f for event %d ",fThresholds[i] , fEvent) );
	        fSignalsBelowThreshold++;
		signalWithOffset.setRecoTimeAtThreshold(fThresholds[i], 0);
		break;
	  }
	  //setting time at threshold for new signal of signal
	signalWithOffset.setRecoTimeAtThreshold(fThresholds[i], timeAtThr);
// 	JPetRecoSignalTools::savePNGOfSignalWithThreshold(signalWithOffset, fEvent, fThresholds[i] );
  }
  //saving singal into output root file
  fWriter->write(signalWithOffset);
  // increase event counter
  fEvent++;
}

void SDARecoTimeAtThr::end()
{
double goodPercent = (fEventNb-fSignalsBelowThreshold) * 100.0/fEventNb;

	INFO(
	Form("Time at threshold calculation complete \nAmount of bad signals: %d \n %f %% of data is good" , fSignalsBelowThreshold, goodPercent) );

}
