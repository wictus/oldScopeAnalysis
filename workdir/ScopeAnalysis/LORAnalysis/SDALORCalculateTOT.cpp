#include "SDALORCalculateTOT.h"
#include "../JPetRecoSignalTools/JPetRecoSignalTools.h"

SDALORCalculateTOT::SDALORCalculateTOT(const char* name, const char* title, const char* in_file_suffix, const char* out_file_suffix, const double threshold)
: JPetCommonAnalysisModule(name, title, in_file_suffix, out_file_suffix), fThreshold( threshold )
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
  JPetLOR newLOR = oldLOR;
 
  JPetRecoSignal signalToBeProcessed = newLOR.getFirstHit().getSignalA().getRecoSignal();  
  double calculatedTOT = JPetRecoSignalTools::calculateTOT( signalToBeProcessed , fThreshold);
  
  if(calculatedTOT < 100)
  {
    fEvent++;
    return;
  }  
  
  JPetPhysSignal signalToChange = oldLOR.getFirstHit().getSignalA();
  signalToChange.setPhe(calculatedTOT);
  JPetHit hitToChange = newLOR.getFirstHit();
  hitToChange.setSignalA( signalToChange );
  
  signalToBeProcessed = newLOR.getFirstHit().getSignalB().getRecoSignal();
  calculatedTOT = JPetRecoSignalTools::calculateTOT( signalToBeProcessed , fThreshold);
  
  if(calculatedTOT < 100)
  {
    fEvent++;
    return;
  }  
  
  signalToChange = oldLOR.getFirstHit().getSignalB();
  signalToChange.setPhe(calculatedTOT);
  hitToChange.setSignalB( signalToChange );
  
  newLOR.setFirstHit(hitToChange);
  
  signalToBeProcessed = newLOR.getSecondHit().getSignalA().getRecoSignal(); 
  calculatedTOT = JPetRecoSignalTools::calculateTOT( signalToBeProcessed , fThreshold);
  
  if(calculatedTOT < 100)
  {
    fEvent++;
    return;
  }  
  
  signalToChange = oldLOR.getSecondHit().getSignalA();
  signalToChange.setPhe(calculatedTOT);
  hitToChange.setSignalA( signalToChange );
  
  signalToBeProcessed = newLOR.getSecondHit().getSignalB().getRecoSignal();
  calculatedTOT = JPetRecoSignalTools::calculateTOT( signalToBeProcessed , fThreshold);
  
  if(calculatedTOT < 100)
  {
    fEvent++;
    return;
  }
  
  signalToChange = oldLOR.getSecondHit().getSignalB();
  signalToChange.setPhe(calculatedTOT);
  hitToChange.setSignalB( signalToChange );
  
  newLOR.setSecondHit(hitToChange);
  
  fWriter->write(newLOR);

  fEvent++;
}
