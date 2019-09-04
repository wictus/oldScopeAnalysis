#include "./SDAMatchLORs.h"
#include "sstream"
#include <stdlib.h>
#include "fstream"

ClassImp(SDAMatchLORs);

SDAMatchLORs::SDAMatchLORs(const char* name, const char* title,
                           const char* in_file_suffix, const char* out_file_suffix) : 
  JPetCommonAnalysisModule(name, title, in_file_suffix, out_file_suffix)
{
    fMatched=0;
    fNotMatched=0;
  setVersion(MODULE_VERSION);
  
}




SDAMatchLORs::~SDAMatchLORs()
{
	
}

void SDAMatchLORs::begin()
{
	INFO(
    Form("Amount of hits in file: %d", fEventNb ) );
    fNotMatched = 0;

    fReader->getEntry(fEvent);
    const JPetHit& hit = (JPetHit&) fReader->getData();
    fTSlot = hit.getSignalA().getTSlotIndex();

}

void SDAMatchLORs::exec()
{
  //Loop over all signals with the samefTSlotIndex
  for(int currentTSlot =fTSlot; currentTSlot ==fTSlot && fEvent <= fEventNb; fEvent++)
  {
        //get entry from file
        fReader->getEntry(fEvent);

        //get signal from entry
        const JPetHit& hit = (JPetHit&) fReader->getData();

        //put signal into fArray
        fArray.push_back(hit);
        currentTSlot = hit.getSignalA().getTSlotIndex();
  }

  if(fEvent > fEventNb){
        fEvent = fEventNb;
        return;
        }

        
  //make sure that last signal is not from nextfTSlotIndex
  if(fArray[fArray.size() -1].getSignalA().getTSlotIndex() !=fTSlot){
        fArray.pop_back();
        fEvent--;
        }

  
  //increasefTSlot;
  fTSlot++; 

//// HANDLE 4 signal stuff  
//   for(unsigned int firstHitIndex = 0; firstHitIndex < fArray.size()-1 && 0 != fArray.size(); firstHitIndex++)
//   {	
// 	std::cout << firstHitIndex << std::endl;
// 	for(unsigned int secondHitIndex = firstHitIndex+1; secondHitIndex < fArray.size(); secondHitIndex++)
// 	{
// 		if(fArray[firstHitIndex].getScintillator().getID() != fArray[secondHitIndex].getScintillator().getID() )
// 		{
// 			JPetLOR lor( 0,0, fArray[firstHitIndex], fArray[secondHitIndex]);
// 			
// // 			std::cout << "PM1: " <<lor.getFirstHit().getSignalA().getPM().getID() << std::endl;
// // 			std::cout << "PM2: " <<lor.getFirstHit().getSignalB().getPM().getID() << std::endl;
// // 			std::cout << "PM3: " <<lor.getSecondHit().getSignalA().getPM().getID() << std::endl;
// // 			std::cout << "PM4: " <<lor.getSecondHit().getSignalB().getPM().getID() << std::endl;
// // 			
// 			fWriter->write(lor);
// 			fMatched++;
// 		}
// 	}
//   }
// HANDLE 2 signal stuff
  if( 0 != fArray.size())
  {	
	JPetHit emptyHit;
	JPetLOR lor( 0,0, fArray[0], emptyHit);
	
// 	std::cout << "PM1: " <<lor.getFirstHit().getSignalA().getPM().getID() << std::endl;
// 	std::cout << "PM2: " <<lor.getFirstHit().getSignalB().getPM().getID() << std::endl;
	
	fWriter->write(lor);
	fMatched++;
	
  }
  fArray.clear();
  
}


void SDAMatchLORs::end()
{
  	INFO(
	Form("Matching complete \nAmount of fMatched hits: %d " , fMatched) );
	double goodPercent = fMatched* 100.0 /fEventNb ;
	INFO(
	Form("%f %% of data was not fMatched \n " , goodPercent) );
}
