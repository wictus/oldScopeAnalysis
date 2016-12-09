#include "./SDALORCutOnTOT.h"

ClassImp(SDALORCutOnTOT);

SDALORCutOnTOT::SDALORCutOnTOT(const char* name, const char* title,
                                       const char* in_file_suffix, const char* out_file_suffix, const double thresholdValue, const int PMTID):
  JPetCommonAnalysisModule(name, title, in_file_suffix, out_file_suffix), 
  fPMTID(PMTID)
{
  setVersion(MODULE_VERSION);
  fTOTThreshold = thresholdValue; 
}

SDALORCutOnTOT::~SDALORCutOnTOT()
{
}

void SDALORCutOnTOT::begin()
{
	fAboveThreshold = 0;
}

void SDALORCutOnTOT::exec()
{

  fReader->getEntry(fEvent);
  const JPetLOR lor = (JPetLOR&) fReader->getData();

  //if( lor.getFirstHit().getEnergy() > fThresholdEnergy && lor.getSecondHit().getEnergy() > fThresholdEnergy && lor.getFirstHit().getEnergy() < upperThreshold && lor.getSecondHit().getEnergy() < upperThreshold)
  if( lor.getFirstHit().getEnergy() > fTOTThreshold&& lor.getSecondHit().getEnergy() > fTOTThreshold)
	{
	  fWriter->write(lor);	
	  fAboveThreshold++;
	}
// increase event counter
  fEvent++;
}


void SDALORCutOnTOT::end()
{
	INFO(Form("%f  events were above threshold", (double)fAboveThreshold/(double)fEventNb 	));
	std::cout << "Events above the threshold TOT: " << fAboveThreshold << std::endl;
}


