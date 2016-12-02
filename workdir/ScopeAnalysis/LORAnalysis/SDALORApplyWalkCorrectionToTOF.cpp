#include "SDALORApplyWalkCorrectionToTOF.h"


SDALORApplyWalkCorrectionToTOF::SDALORApplyWalkCorrectionToTOF(const char* name, const char* title, const char* in_file_suffix, const char* out_file_suffix, const double coefficient)
: JPetCommonAnalysisModule(name, title, in_file_suffix, out_file_suffix), fCoefficient( coefficient )
{

}

SDALORApplyWalkCorrectionToTOF::~SDALORApplyWalkCorrectionToTOF()
{

}

void SDALORApplyWalkCorrectionToTOF::begin()
{

}

void SDALORApplyWalkCorrectionToTOF::end()
{
  TH1F* histo = new TH1F("","", 200, -1000,1000);
  for(unsigned int i = 0; i < fTOF.size(); i++)
  {
    histo->Fill(fTOF[i]);
  }
  histo->Fit("gaus");
  gStyle->SetOptFit(1);
  TCanvas* c = new TCanvas();
  histo->Draw();
  std::stringstream buf;
  buf << fHeader->getSourcePosition() << "TOF.png";
  c->SaveAs( buf.str().c_str() );
}

void SDALORApplyWalkCorrectionToTOF::exec()
{
  fReader->getEntry(fEvent);
  //exctract data for each strip seperately
  JPetLOR& oldLOR = dynamic_cast< JPetLOR& > ( fReader->getData() );

  JPetLOR newLOR = oldLOR;
  
  double invertedCharges = 1.0/sqrt( oldLOR.getFirstHit().getSignalA().getRecoSignal().getCharge() );
  invertedCharges+=1.0/sqrt( oldLOR.getFirstHit().getSignalB().getRecoSignal().getCharge() );
  invertedCharges-=1.0/sqrt( oldLOR.getSecondHit().getSignalA().getRecoSignal().getCharge() );
  invertedCharges-=1.0/sqrt( oldLOR.getSecondHit().getSignalB().getRecoSignal().getCharge() );
  
  newLOR.setTimeDiff( oldLOR.getTimeDiff() - fCoefficient*invertedCharges );

  fTOF.push_back( newLOR.getTimeDiff() );
  
  fWriter->write(newLOR);
  
  fEvent++;
}

