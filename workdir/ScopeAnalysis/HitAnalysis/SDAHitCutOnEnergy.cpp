#include "./SDAHitCutOnEnergy.h"

ClassImp(SDAHitCutOnEnergy);

SDAHitCutOnEnergy::SDAHitCutOnEnergy(const char* name, const char* title,
                                       const char* in_file_suffix, const char* out_file_suffix, const double thresholdValue) :
  JPetCommonAnalysisModule(name, title, in_file_suffix, out_file_suffix)
{
  setVersion(MODULE_VERSION);
  fThresholdEnergy = thresholdValue; 
}

SDAHitCutOnEnergy::~SDAHitCutOnEnergy()
{
}

void SDAHitCutOnEnergy::begin()
{
	fAboveThreshold = 0;
}

void SDAHitCutOnEnergy::exec()
{

  fReader->getEntry(fEvent);
  const JPetHit hit = (JPetHit&) fReader->getData();

  if( hit.getEnergy() > fThresholdEnergy )
	{
	  fWriter->write(hit);	
	  fAboveThreshold++;
	}
// increase event counter
  fEvent++;
}


void SDAHitCutOnEnergy::end()
{
	INFO(Form("%f  events were above threshold", (double)fAboveThreshold/(double)fEventNb 	));
}


