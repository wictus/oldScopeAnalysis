#include "./SDAMakePhysSignals.h"
#include "../JPetRecoSignalTools/JPetRecoSignalTools.h"
#include "sstream"
#include <stdlib.h>
#include "fstream"

ClassImp(SDAMakePhysSignals);

SDAMakePhysSignals::SDAMakePhysSignals(const char* name, const char* title, const char* in_file_suffix, const char* out_file_suffix, std::map< int, TF1 > gainCurves, std::map< int, double > voltages)
: JPetCommonAnalysisModule(name, title, in_file_suffix, out_file_suffix), fVoltages( voltages ), fGainCurves( gainCurves )
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
//   WARNING( Form("This module currently sets number of photoelectrons equal to charge of JPetRecoSignal!")  );
//   if(fVoltages.size() == 4 && fGainCurves.size() == 4)
//   {
//     double singlePhe = fGainCurves[physSignal.getPM().getID()].Eval( fVoltages[ physSignal.getPM().getID() ] );
//     double phe = physSignal.getRecoSignal().getCharge() / singlePhe;
//      physSignal.setPhe( phe );
//   }
//   else
    physSignal.setPhe( physSignal.getRecoSignal().getCharge() );

  fWriter->write(physSignal);
// increase event counter
  fEvent++;
}


void SDAMakePhysSignals::end()
{
}
