#include "./SDAMakePhysSignals.h"
#include "../JPetRecoSignalTools/JPetRecoSignalTools.h"
#include "sstream"
#include <stdlib.h>
#include "fstream"

ClassImp(SDAMakePhysSignals);

SDAMakePhysSignals::SDAMakePhysSignals(const char* name, const char* title,
                                       const char* in_file_suffix, const char* out_file_suffix) :
  JPetCommonAnalysisModule(name, title, in_file_suffix, out_file_suffix)
{
  setVersion(MODULE_VERSION);
}

SDAMakePhysSignals::~SDAMakePhysSignals()
{
}

void SDAMakePhysSignals::begin()
{
}

void SDAMakePhysSignals::exec()
{

  fReader->getEntry(fEvent);
  const JPetRecoSignal signal = (JPetRecoSignal&) fReader->getData();

  JPetPhysSignal physSignal;
  physSignal.setRecoSignal(signal);
  WARNING( Form("This module currently sets number of photoelectrons equal to charge of JPetRecoSignal!")  );
  physSignal.setPhe(physSignal.getRecoSignal().getCharge() );

  fWriter->write(physSignal);
// increase event counter
  fEvent++;
}


void SDAMakePhysSignals::end()
{
}
