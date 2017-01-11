#include "./SDALORCalculateTOT.h"
#include "../JPetRecoSignalTools/JPetRecoSignalTools.h"
#include "../../../framework/JPetManager/JPetManager.h"

ClassImp(SDALORCalculateTOT);

SDALORCalculateTOT::SDALORCalculateTOT(const char* name, const char* title, const char* in_file_suffix, const char* out_file_suffix, const double threshold)
: JPetCommonAnalysisModule(name, title, in_file_suffix, out_file_suffix),
  fThreshold( threshold )
{

}

SDALORCalculateTOT::~SDALORCalculateTOT()
{

}

void SDALORCalculateTOT::begin()
{

}

void SDALORCalculateTOT::end()
{

}

void SDALORCalculateTOT::exec()
{
  fReader->getEntry(fEvent);

  JPetLOR& oldLOR = dynamic_cast< JPetLOR& > ( fReader->getData() );
  JPetLOR newLOR;
  
  JPetPhysSignal signalA = swapChargeToTOTInPhysSignal(oldLOR.getFirstHit().getSignalA());  
  JPetPhysSignal signalB = swapChargeToTOTInPhysSignal(oldLOR.getFirstHit().getSignalB());
  JPetPhysSignal signalC = swapChargeToTOTInPhysSignal(oldLOR.getSecondHit().getSignalA());      
  JPetPhysSignal signalD = swapChargeToTOTInPhysSignal(oldLOR.getSecondHit().getSignalB());      
  
  if(signalA.getPhe() < 100 || signalB.getPhe() < 100 || signalC.getPhe() < 100 || signalD.getPhe() < 100)
  {
    fEvent++;
    return;
  }  
  
  JPetHit hitToChangeA = oldLOR.getFirstHit();
  hitToChangeA.setSignalA( signalA );
  hitToChangeA.setSignalB( signalB );
  
  JPetHit hitToChangeB = oldLOR.getSecondHit();
  hitToChangeB.setSignalA( signalC );
  hitToChangeB.setSignalB( signalD );
  
  newLOR.setFirstHit(hitToChangeA);
  newLOR.setSecondHit(hitToChangeB);
  
  fWriter->write(newLOR);

  fEvent++;
}

JPetPhysSignal SDALORCalculateTOT::swapChargeToTOTInPhysSignal(JPetPhysSignal signal)
{
  double TOT = JPetRecoSignalTools::calculateTOT( signal.getRecoSignal(), fThreshold); 
  signal.setPhe( TOT );
  return signal;
}
