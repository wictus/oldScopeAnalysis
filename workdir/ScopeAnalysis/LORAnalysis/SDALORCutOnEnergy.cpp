#include "./SDALORCutOnEnergy.h"

ClassImp(SDALORCutOnEnergy);

SDALORCutOnEnergy::SDALORCutOnEnergy(const char* name, const char* title,
                                       const char* in_file_suffix, const char* out_file_suffix, const double thresholdValue) :
  JPetCommonAnalysisModule(name, title, in_file_suffix, out_file_suffix)
{
  setVersion(MODULE_VERSION);
  fThresholdEnergy = thresholdValue; 
}

SDALORCutOnEnergy::~SDALORCutOnEnergy()
{
}

void SDALORCutOnEnergy::begin()
{
	fAboveThreshold = 0;
}

void SDALORCutOnEnergy::exec()
{

  fReader->getEntry(fEvent);
  const JPetLOR lor = (JPetLOR&) fReader->getData();

  //if( lor.getFirstHit().getEnergy() > fThresholdEnergy && lor.getSecondHit().getEnergy() > fThresholdEnergy && lor.getFirstHit().getEnergy() < upperThreshold && lor.getSecondHit().getEnergy() < upperThreshold)
  if( lor.getFirstHit().getEnergy() > fThresholdEnergy && lor.getSecondHit().getEnergy() > fThresholdEnergy)
	{
	  fWriter->write(lor);	
	  fAboveThreshold++;
	}
// increase event counter
  fEvent++;
}


void SDALORCutOnEnergy::end()
{
	INFO(Form("%f  events were above threshold", (double)fAboveThreshold/(double)fEventNb 	));
	std::cout << "Events above the threshold energy: " << fAboveThreshold << std::endl;
}


