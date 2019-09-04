#include "SDALORApplyWalkCorrectionToDeltaT.h"

SDALORApplyWalkCorrectionToDeltaT::SDALORApplyWalkCorrectionToDeltaT(const char* name, const char* title, const char* in_file_suffix, const char* out_file_suffix, std::map< int, double > coefficient)
: JPetCommonAnalysisModule(name, title, in_file_suffix, out_file_suffix), fCoefficient( coefficient )
{

}


SDALORApplyWalkCorrectionToDeltaT::~SDALORApplyWalkCorrectionToDeltaT()
{

}

void SDALORApplyWalkCorrectionToDeltaT::begin()
{

}

void SDALORApplyWalkCorrectionToDeltaT::end()
{
  
}

void SDALORApplyWalkCorrectionToDeltaT::exec()
{
  fReader->getEntry(fEvent);
  //exctract data for each strip seperately
  JPetLOR& oldLOR = dynamic_cast< JPetLOR& > ( fReader->getData() );

  JPetLOR newLOR = oldLOR;
  
  JPetHit firstHit = oldLOR.getFirstHit();
  
  double invertedCharges = 1.0/sqrt( oldLOR.getFirstHit().getSignalA().getRecoSignal().getCharge() );
  invertedCharges-=1.0/ sqrt( oldLOR.getFirstHit().getSignalB().getRecoSignal().getCharge() );
  
  firstHit.setTimeDiff( firstHit.getTimeDiff() - fCoefficient[firstHit.getSignalA().getRecoSignal().getPM().getID()]* invertedCharges);
  newLOR.setFirstHit(firstHit);
  
  JPetHit secondHit = oldLOR.getSecondHit();
  
  invertedCharges=1.0/sqrt( oldLOR.getSecondHit().getSignalA().getRecoSignal().getCharge() );
  invertedCharges-=1.0/sqrt( oldLOR.getSecondHit().getSignalB().getRecoSignal().getCharge() );
  
  secondHit.setTimeDiff( secondHit.getTimeDiff() - fCoefficient[secondHit.getSignalA().getRecoSignal().getPM().getID()]* invertedCharges ); 
  
  newLOR.setSecondHit(secondHit);
  
  fWriter->write(newLOR);
  
  fEvent++;
}

