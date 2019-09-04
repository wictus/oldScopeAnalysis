#include "SDALORCalculateEnergyFromCalibration.h"

SDALORCalculateEnergyFromCalibration::SDALORCalculateEnergyFromCalibration(const char* name, const char* title, const char* in_file_suffix, const char* out_file_suffix, std::map< int, std::vector< double > > energyCalibration)
:JPetCommonAnalysisModule(name, title, in_file_suffix, out_file_suffix), fCalibration(energyCalibration)
{

}

SDALORCalculateEnergyFromCalibration::~SDALORCalculateEnergyFromCalibration()
{

}

void SDALORCalculateEnergyFromCalibration::begin()
{
  const JPetParamBank& bank = getParamBank();
  for(int i = 0; i < bank.getScintillatorsSize(); i++)
  {
    TF1* function = new TF1("function","[0]+[1]*x+[2]*x*x",0,300);
    function->SetParameter(0, fCalibration[ bank.getScintillator(i).getID() ][0]);
    function->SetParameter(1, fCalibration[ bank.getScintillator(i).getID() ][1]);
    function->SetParameter(2, fCalibration[ bank.getScintillator(i).getID() ][2]);
    fCalibrationFunctions[ bank.getScintillator(i).getID() ] = function;
  }

}

void SDALORCalculateEnergyFromCalibration::end()
{

}

void SDALORCalculateEnergyFromCalibration::exec()
{
  fReader->getEntry(fEvent);

  JPetLOR& fLOR = dynamic_cast< JPetLOR& > ( fReader->getData() );
  
  JPetLOR newLOR = fLOR;
  
  JPetHit hit = fLOR.getFirstHit();
  double position = hit.getPosAlongStrip();
  
  double alpha = (fCalibrationFunctions[ hit.getSignalA().getRecoSignal().getPM().getScin().getID() ])->Eval(position);
  
  double energy = (hit.getSignalA().getRecoSignal().getCharge()+hit.getSignalB().getRecoSignal().getCharge()) / 2.0 * alpha;
  
  hit.setEnergy(energy);
  
  newLOR.setFirstHit(hit);

  hit = fLOR.getSecondHit();
  position = hit.getPosAlongStrip();
  
  alpha = (fCalibrationFunctions[ hit.getSignalA().getRecoSignal().getPM().getScin().getID() ])->Eval(position);
  
  energy = (hit.getSignalA().getRecoSignal().getCharge()+hit.getSignalB().getRecoSignal().getCharge()) / 2.0 * alpha;
  
  hit.setEnergy(energy);
  
  newLOR.setSecondHit(hit);
  
  fWriter->write(newLOR);

  fEvent++;
  
}

