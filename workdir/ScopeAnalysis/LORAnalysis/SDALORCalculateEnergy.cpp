#include "./SDALORCalculateEnergy.h"

ClassImp(SDALORCalculateEnergy);

SDALORCalculateEnergy::SDALORCalculateEnergy(const char* name, const char* title,
                                       const char* in_file_suffix, const char* out_file_suffix, std::vector< std::pair <int, double>> calibration) :
  JPetCommonAnalysisModule(name, title, in_file_suffix, out_file_suffix)
{
  fCalibration = calibration;
  setVersion(MODULE_VERSION);
}

SDALORCalculateEnergy::~SDALORCalculateEnergy()
{
}

void SDALORCalculateEnergy::begin()
{
}

void SDALORCalculateEnergy::exec()
{

  fReader->getEntry(fEvent);
  const JPetLOR lor = (JPetLOR&) fReader->getData();

  JPetLOR lorWithEnergy = lor;

  for(unsigned int i = 0; i < fCalibration.size(); i++)
  {
	if( lor.getFirstHit().getScintillator().getID() == fCalibration[i].first )
	{
	   double chargeOne = lor.getFirstHit().getSignalA().getPhe();
	   double chargeTwo = lor.getFirstHit().getSignalB().getPhe();
	   JPetHit hit = lorWithEnergy.getFirstHit();
	   hit.setEnergy( (chargeOne + chargeTwo)/2.0 * fCalibration[i].second );
	   lorWithEnergy.setFirstHit( hit );
	}
	else if( lor.getSecondHit().getScintillator().getID() == fCalibration[i].first )
	{
	   double chargeOne = lor.getSecondHit().getSignalA().getPhe();
	   double chargeTwo = lor.getSecondHit().getSignalB().getPhe();
	   JPetHit hit = lorWithEnergy.getSecondHit();
	   hit.setEnergy( (chargeOne + chargeTwo)/2.0 * fCalibration[i].second );
	   lorWithEnergy.setSecondHit( hit );
	}
	else
	{
		WARNING( Form("DID NOT FIND SCINTILLATOR IN GIVEN CALIBRATION VECTOR!")  );
	}
  }

  fWriter->write(lorWithEnergy);
// increase event counter
  fEvent++;
}


void SDALORCalculateEnergy::end()
{
}

