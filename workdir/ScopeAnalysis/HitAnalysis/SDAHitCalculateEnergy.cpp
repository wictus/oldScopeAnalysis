#include "./SDAHitCalculateEnergy.h"

ClassImp(SDAHitCalculateEnergy);

SDAHitCalculateEnergy::SDAHitCalculateEnergy(const char* name, const char* title,
                                       const char* in_file_suffix, const char* out_file_suffix) :
  JPetCommonAnalysisModule(name, title, in_file_suffix, out_file_suffix)
{
  setVersion(MODULE_VERSION);
}

SDAHitCalculateEnergy::~SDAHitCalculateEnergy()
{
}

void SDAHitCalculateEnergy::begin()
{
}

void SDAHitCalculateEnergy::exec()
{

  fReader->getEntry(fEvent);
  const JPetHit hit = (JPetHit&) fReader->getData();

  JPetHit hitWithEnergy = hit;
  fEnergyFactor = hit.getQualityOfTime();

  fEnergy = ( hit.getSignalA().getPhe() + hit.getSignalB().getPhe() ) /2.0 * fEnergyFactor;
  hitWithEnergy.setEnergy( fEnergy );

  fWriter->write(hitWithEnergy);
// increase event counter
  fEvent++;
}


void SDAHitCalculateEnergy::end()
{
}

