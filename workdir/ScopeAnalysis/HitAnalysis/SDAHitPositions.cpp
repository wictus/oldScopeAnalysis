#include "./SDAHitPositions.h"
#include "sstream"
#include <stdlib.h>
#include "fstream"

ClassImp(SDAHitPositions);

SDAHitPositions::SDAHitPositions(const char* name, const char* title,
                                 const char* in_file_suffix, const char* out_file_suffix ,
                                 const double threshold, const double velocity) :
  JPetCommonAnalysisModule(name, title, in_file_suffix, out_file_suffix)
{
	fGivenThreshold = threshold;
	fGivenVelocity = velocity;
  setVersion(MODULE_VERSION);
}

SDAHitPositions::~SDAHitPositions()
{
}

void SDAHitPositions::begin()
{
	INFO(
      Form("Amount of signals in file: %d", fEventNb ) );
}

void SDAHitPositions::exec()
{

  // Get a hit
  fReader->getEntry(fEvent);
  const JPetHit& hit = (JPetHit&) fReader->getData();
  JPetHit hitToSave = hit;

  hitToSave.setTimeDiff(hit.getSignalA().getRecoSignal().getRecoTimeAtThreshold(fGivenThreshold) - hit.getSignalB().getRecoSignal().getRecoTimeAtThreshold(fGivenThreshold) );

  hitToSave.setPosAlongStrip(hitToSave.getTimeDiff() * fGivenVelocity / 2.0 ); // constant taken from scan at middle position

  hitToSave.setTime( ( hit.getSignalA().getRecoSignal().getRecoTimeAtThreshold(fGivenThreshold) + hit.getSignalB().getRecoSignal().getRecoTimeAtThreshold(fGivenThreshold) ) / 2.0 );

  fWriter->write(hitToSave);
  fEvent++;
}


void SDAHitPositions::end()
{
	INFO(
	Form("Calculation of hit position complete!") );
}
