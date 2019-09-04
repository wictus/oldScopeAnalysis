#include "SDAHitFindEnergyConstant.h"
#include "../JPetHitTools/FindConstant.h"

ClassImp(SDAHitFindEnergyConstant);

SDAHitFindEnergyConstant::SDAHitFindEnergyConstant(const char* name, const char* title,
                           const char* in_file_suffix, const char* out_file_suffix) : JPetCommonAnalysisModule(name, title, in_file_suffix, out_file_suffix)
{
  std::cout << "In SDAHitFindEnergyConstant::constructor \n";
}

void SDAHitFindEnergyConstant::begin()
{	
}

SDAHitFindEnergyConstant::~SDAHitFindEnergyConstant()
{

}
void SDAHitFindEnergyConstant::exec()
{
      
	const JPetParamBank& bank = getParamBank();
	
	if(fEvent < fEventNb ){
		
		for(int i = 0; i < bank.getScintillatorsSize(); i++)
		{
			fScintillatorID = bank.getScintillator(i).getID();
               		std::string filePath = (std::string)fHeader->getBaseFileName();
			
			INFO( Form( "Extracting data for file: %s for position: %f scintillator: %d", filePath.c_str(),  fHeader->getSourcePosition(), fScintillatorID) ) ;
			for(fEvent = 0; fEvent< fEventNb; fEvent++)
			{
				fReader->getEntry(fEvent);
				const JPetHit& hit = (JPetHit&) fReader->getData();
				if(hit.getScintillator().getID() == fScintillatorID )
				{
				  std::cout << (hit.getSignalA().getPhe() + hit.getSignalB().getPhe() ) /2.0 << std::endl;
					fEXPEvents.push_back( (hit.getSignalA().getPhe() + hit.getSignalB().getPhe() ) /2.0 );
				}
			}
			FindConstant finderOfEnergy(fEXPEvents, fHeader->getBaseFileName(), fHeader->getSourcePosition(), fScintillatorID);
			fEnergyConstants.push_back( finderOfEnergy.execute() );
			fEnergyResolutions.push_back( finderOfEnergy.returnEnergyResolution() );
			
			fEXPEvents.clear();
			INFO(Form( "Finished fit for scintillator: %d", fScintillatorID) );
			
		}

		for(fEvent = 0; fEvent< fEventNb; fEvent++)
		{
			fReader->getEntry(fEvent);
			const JPetHit& hitOriginal = (JPetHit&) fReader->getData();
			JPetHit hit = hitOriginal;
			for(int i = 0; i < bank.getScintillatorsSize(); i++){
				fScintillatorID = bank.getScintillator(i).getID();
				if( hit.getScintillator().getID() == fScintillatorID )
				{
					hit.setQualityOfEnergy( fEnergyResolutions[i] );
					hit.setQualityOfTime( fEnergyConstants[i] );
					fWriter->write(hit);
				}
			}
		 }
	}

}

void SDAHitFindEnergyConstant::end()
{
	//tu jeszcze brakuje zapisu przeskalowań do pliku TXT wczytywanego przez kolejny moduł
	//format ID scyntylatora      alfa

	//wpisać rozdzielczości energetyczne do pasków
} 

