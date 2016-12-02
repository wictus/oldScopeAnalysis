#include "SDALORExctractAmplitudes.h"

ClassImp(SDALORExctractAmplitudes);

SDALORExctractAmplitudes::SDALORExctractAmplitudes(const char* name, const char* title,
                           const char* in_file_suffix, const char* out_file_suffix) : JPetCommonAnalysisModule(name, title, in_file_suffix, out_file_suffix)
{
}

void SDALORExctractAmplitudes::begin()
{
}

SDALORExctractAmplitudes::~SDALORExctractAmplitudes()
{

}
void SDALORExctractAmplitudes::exec()
{
	const JPetParamBank& bank = getParamBank();
	
	if(fEvent < fEventNb ){
		
		for(int i = 0; i < bank.getPMsSize(); i++)
		{
			fPMTID = bank.getPM(i).getID();
               		std::string filePath = (std::string)fHeader->getBaseFileName();
			
			INFO( Form( "Extracting data for file: %s for position: %f photomultiplier: %d", filePath.c_str(),  fHeader->getSourcePosition(), fPMTID ) ) ;
			for(fEvent = 0; fEvent< fEventNb; fEvent++)
			{
				fReader->getEntry(fEvent);
				const JPetLOR& lor = (JPetLOR&) fReader->getData();
				const JPetHit& hitA = lor.getFirstHit();
				const JPetHit& hitB = lor.getSecondHit();
				const JPetRecoSignal& signalAA = hitA.getSignalA().getRecoSignal();
				const JPetRecoSignal& signalAB = hitA.getSignalB().getRecoSignal();
				const JPetRecoSignal& signalBA = hitB.getSignalA().getRecoSignal();
				const JPetRecoSignal& signalBB = hitB.getSignalB().getRecoSignal();
				std::vector<JPetRecoSignal> signals;
				signals.push_back(signalAA);
				signals.push_back(signalAB);
				signals.push_back(signalBA);
				signals.push_back(signalBB);
				for(unsigned int i = 0; i < signals.size(); i++)
				{
				  if( signals[i].getPM().getID() == fPMTID  )
				  {
					fEXPEvents.push_back( signals[i].getAmplitude() );
				  }
				}
			}
			std::stringstream buf;
			buf << fPMTID << "_ampltudes.txt";
			saveToFile( buf.str(), fEXPEvents );
			fEXPEvents.clear();
			INFO(Form( "Finished fit for scintillator: %d", fPMTID ) );
			
		}

	}

}

void SDALORExctractAmplitudes::saveToFile(std::string fileName, std::vector<double>& data)
{
  std::ofstream outFile;
  outFile.open( fileName.c_str() );
  for( unsigned int i = 0 ; i < data.size(); i++ )
  {
      outFile << data[i] << std::endl;
  }
  outFile.close();
}

void SDALORExctractAmplitudes::end()
{
	//tu jeszcze brakuje zapisu przeskalowań do pliku TXT wczytywanego przez kolejny moduł
	//format ID scyntylatora      alfa

	//wpisać rozdzielczości energetyczne do pasków
} 