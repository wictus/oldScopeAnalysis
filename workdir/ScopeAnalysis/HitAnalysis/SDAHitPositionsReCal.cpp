#include "./SDAHitPositionsReCal.h"
#include "sstream"
#include <stdlib.h>
#include "fstream"

ClassImp(SDAHitPositionsReCal);

SDAHitPositionsReCal::SDAHitPositionsReCal(const char* name, const char* title,
    const char* in_file_suffix, const char* out_file_suffix, double recalTop, double recalBottom) :
  JPetCommonAnalysisModule(name, title, in_file_suffix, out_file_suffix)
{
  setVersion(MODULE_VERSION);
  fReTop = recalTop;
  fReBot = recalBottom;
}

SDAHitPositionsReCal::~SDAHitPositionsReCal()
{
}

void SDAHitPositionsReCal::begin()
{
}

void SDAHitPositionsReCal::exec()
{

  // Get a LOR Signal
  fReader->getEntry(fEvent);
  const JPetHit& hit = (JPetHit&) fReader->getData();
  JPetHit hitToSave = hit;

//	std::cout<<"Before looP\n";
//	std::cout<< " hitToSave.getSignalA().getPM().getScin().getID() " << hitToSave.getSignalB().getPM().getID() << std::endl;
  if ( hitToSave.getSignalA().getPM().getScin().getID() == 72) {
//		std::cout<<"in if\n";

    hitToSave.setPosAlongStrip(hit.getTimeDiff() * 12.6 / 1000 / 2 - fReTop ); // constant taken from scan at middle position

  }
  if ( hitToSave.getSignalA().getPM().getScin().getID() == 71) {
//		std::cout<<"in else\n";
    hitToSave.setPosAlongStrip(hit.getTimeDiff() * 12.6 / 1000 / 2 - fReBot ); // constant taken from scan at middle position
  }

  fWriter->write(hitToSave);
  fEvent++;
}


void SDAHitPositionsReCal::end()
{
}
