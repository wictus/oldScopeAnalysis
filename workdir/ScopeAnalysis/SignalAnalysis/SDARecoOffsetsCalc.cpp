#include "./SDARecoOffsetsCalc.h"
#include "../JPetRecoSignalTools/JPetRecoSignalTools.h"
#include "sstream"
#include <stdlib.h>
#include "../../../framework/JPetManager/JPetManager.h"

ClassImp(SDARecoOffsetsCalc);

//standard constructor from AnalysisModule
SDARecoOffsetsCalc::SDARecoOffsetsCalc(const char* name, const char* title,
                   const char* in_file_suffix, const char* out_file_suffix) 
                   : JPetCommonAnalysisModule(name, title, in_file_suffix, out_file_suffix)
{
  setVersion(MODULE_VERSION);
}

//no specific destructor needed
SDARecoOffsetsCalc::~SDARecoOffsetsCalc()
{
}

//Checking how many signals are stored in file and writing it on screen
void SDARecoOffsetsCalc::begin()
{
      INFO(
      Form("Amount of signals in file: %d", fEventNb ) );
      fBadSignals = 0;
}

//used for taking RecoSignals from file, calculating fOffset and saving new
//RecoSignals with fOffset values into new file
void SDARecoOffsetsCalc::exec()
{

  // Take next entry 
  fReader->getEntry(fEvent);
  
  // Cast data from the entry into JPetRecoSignal
  const JPetRecoSignal& signal = (JPetRecoSignal&) fReader->getData();
  //fOffset calculation
  fOffset = JPetRecoSignalTools::calculateOffset(signal);
  
  // if something went wrong with calculating fOffset its value will be equal to badOffset value
  // and save bad signal plot into root file
  if ( fOffset == JPetRecoSignalTools::ERRORS::badOffset ) {
	WARNING( Form("Problem with calculating fOffset for event: %d",fEvent) );
//  	JPetRecoSignalTools::saveBadSignalIntoRootFile(signal, fBadSignals, "badOffsets.root");
	fBadSignals++;
	fEvent++;
	return;
  }

  //to save Reco signal one needs to copy it to non const variable
  JPetRecoSignal signalWithOffset = signal;
//  JPetRecoSignalTools::savePNGOfBadSignal(signal, fEvent);
  //setting fOffset of signal
  signalWithOffset.setOffset(fOffset);
  //saving singal into output root file
  fWriter->write(signalWithOffset);

  // increase event counter
  fEvent++;
}

void SDARecoOffsetsCalc::end()
{
	
	double goodPercent = (fEventNb-fBadSignals) * 100.0/fEventNb;
	INFO(
	Form("Offset calculation complete \nAmount of bad signals: %d \n %f %% of data is good" , fBadSignals, goodPercent) );
}
