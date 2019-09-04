#include "./SDARecoCutAroundPhotopeak.h"
#include "../JPetRecoSignalTools/JPetRecoSignalTools.h"
#include "../../../framework/JPetManager/JPetManager.h"

ClassImp(SDARecoCutAroundPhotopeak);


//standard constructor
SDARecoCutAroundPhotopeak::SDARecoCutAroundPhotopeak(const char* name, const char* title,
                   const char* in_file_suffix, const char* out_file_suffix, std::map< std::string, std::pair < double, double > >  photoPeak)
                   : JPetCommonAnalysisModule(name, title, in_file_suffix, out_file_suffix), fChargeCuts( photoPeak )
{
  setVersion(MODULE_VERSION);
}

//no specific destructor needed
SDARecoCutAroundPhotopeak::~SDARecoCutAroundPhotopeak()
{
}

void SDARecoCutAroundPhotopeak::begin()
{
  fFileName = fInFileName;
}

void SDARecoCutAroundPhotopeak::exec()
{
  // Take next entry 
  fReader->getEntry(fEvent);

  // Cast data from the entry into JPetRecoSignal
  const JPetRecoSignal& signal = (JPetRecoSignal&) fReader->getData();

  std::string voltage =  fFileName.substr( fFileName.find_last_of("V") - 4, 4 ); 
  
  int amountOfSigmas = 3;
  
  double upperCut = fChargeCuts[ voltage ].first + amountOfSigmas*fChargeCuts[ voltage ].second;
  double lowerCut = fChargeCuts[ voltage ].first - amountOfSigmas*fChargeCuts[ voltage ].second;
  
  if( signal.getPM().getID() == 4 )
  {
//     std::cout << "ID: " << signal.getPM().getID() << " for event " << fEvent << std::endl;
    if ( signal.getCharge() < upperCut && signal.getCharge() > lowerCut )
    {
//       std::cout << "Passed cut for 4\n";
      fWriter->write(signal);	
    }
    
  }
  else if( signal.getPM().getID() == 3 )
  {
//     std::cout << "event has charge: " << signal.getCharge() << std::endl;
//     std::cout << "ID: " << signal.getPM().getID() << " for event " << fEvent << std::endl;
//     if( signal.getCharge() < (3.0* 10.07+ 106.96) && signal.getCharge() > ( 106.96 - 3.0* 10.07) )
      if( signal.getCharge() < 40&& signal.getCharge() > 20 )
    {
//       std::cout << "Passed cut for 3 \n";
      fWriter->write(signal);
    }
  } 
  else
  {
//     std::cout << "ID: " << signal.getPM().getID() << " for event " << fEvent << std::endl;
    fWriter->write(signal);	
  }
  
  fEvent++;
  

}

void SDARecoCutAroundPhotopeak::end()
{


}

