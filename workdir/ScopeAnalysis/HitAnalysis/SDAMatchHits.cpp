#include "./SDAMatchHits.h"
#include "sstream"
#include <stdlib.h>
#include "fstream"

ClassImp(SDAMatchHits);

SDAMatchHits::SDAMatchHits(const char* name, const char* title,
                           const char* in_file_suffix, const char* out_file_suffix) :
  JPetCommonAnalysisModule(name, title, in_file_suffix, out_file_suffix)
{
  setVersion(MODULE_VERSION);
}



SDAMatchHits::~SDAMatchHits()
{
}

void SDAMatchHits::begin()
{
	INFO(
      Form("Amount of signals in file: %d", fEventNb ) );
      fNotMatched = 0; 
      
      fReader->getEntry(fEvent);
      const JPetPhysSignal& signal = (JPetPhysSignal&) fReader->getData();
      fTSlot = signal.getTSlotIndex();
      fMatched = 0;

	//fill fSegragatedSignals as many times as there are Scints in paramBank
	const JPetParamBank& paramBank = getParamBank();

	for(int i = 0; i < paramBank.getScintillatorsSize(); i++)
	{
		std::vector<JPetPhysSignal> k;
		fSegragatedSignals.push_back( k );
	}
}

void SDAMatchHits::exec()
{
  //Loop over all signals with the samefTSlotIndex
  for(int currentTSlot = fTSlot; currentTSlot == fTSlot && fEvent <= fEventNb; fEvent++)
  {
  	//get entry from file
  	fReader->getEntry(fEvent);

  	//get signal from entry
	const JPetPhysSignal& signal = (JPetPhysSignal&) fReader->getData();
// 	std::cout << signal.getPM().getID() << std::endl;
	//put signal into fArray
	fArray.push_back(signal);
	currentTSlot = signal.getTSlotIndex();
	
  }

  //stop when all events were processed
  if(fEvent > fEventNb){
	fEvent = fEventNb;
	return;
	}

  //make sure that last signal is not from nextfTSlotIndex
  if(fArray[fArray.size() -1].getTSlotIndex() !=fTSlot){
	fArray.pop_back();
	fEvent--;
	}
  
  //increasefTSlot;
  fTSlot++; 
 
  const JPetParamBank& paramBank = getParamBank();

  //Seperate signals from different stripes
  for( int i = 0; i < paramBank.getScintillatorsSize(); i++)
  {
	for( size_t j = 0; j < fArray.size(); j++)
	{
		if( fArray[j].getPM().getScin().getID() == paramBank.getScintillator(i).getID() ){
			fSegragatedSignals[i].push_back(fArray[j]);	
		}
	}
  }
    
  //discard fArrays with different number of signals than two for each stripe since they cannot be merged into JPetHit and later for JPetLOR
  for( size_t i = 0; i < fSegragatedSignals.size() ; i++ )
  {
    if ( fSegragatedSignals[i].size() == 2 ) {
	}
    else{
      fSegragatedSignals.erase(fSegragatedSignals.begin()+i);
      fNotMatched++;
	}	
   }

  //making hits out of fMatched signals
  for( size_t stripeIndex = 0; stripeIndex < fSegragatedSignals.size(); stripeIndex++)
  {
		bool signalAset = false;
		bool signalBset = false;
		JPetHit hit;
	for( size_t signalIndex = 0; signalIndex < fSegragatedSignals[ stripeIndex ].size(); signalIndex++)
	{
		for( int PMIndex = 0; PMIndex < paramBank.getPMsSize(); PMIndex++)
		{
		      
			if( (fSegragatedSignals[ stripeIndex ][ signalIndex ].getPM().getID() == paramBank.getPM(PMIndex).getID()) && PMIndex%2){
				hit.setSignalA(fSegragatedSignals[ stripeIndex ][ signalIndex ]);
				signalAset = true;
				}
			else if( (fSegragatedSignals[ stripeIndex ][ signalIndex ].getPM().getID() == paramBank.getPM( PMIndex ).getID()) && (PMIndex+1)%2) {
				hit.setSignalB(fSegragatedSignals[ stripeIndex ][ signalIndex ]);
				signalBset = true;
				}
		}	
			if( signalAset && signalBset )
			{
				hit.setScintillator(fSegragatedSignals[ stripeIndex ][ signalIndex ].getPM().getScin() );	
				
				fWriter->write(hit);	
				fMatched++;
				break;
			}
              
	}
  }

	fArray.clear();
	for(size_t i = 0; i < fSegragatedSignals.size(); i++)
	{
		fSegragatedSignals[i].clear();
	}

	if(fSegragatedSignals.size() < (size_t)paramBank.getScintillatorsSize())
	{
		for(size_t i = 0; i < (size_t)paramBank.getScintillatorsSize() - fSegragatedSignals.size(); i++)
		{
	                std::vector<JPetPhysSignal> k;
	                fSegragatedSignals.push_back( k );
		}
	}  
}


void SDAMatchHits::end()
{
  	INFO(
	Form("Matching complete \nAmount of fMatched hits: %d " , fMatched) );
	double goodPercent = fNotMatched* 100.0 /fMatched ;
	INFO(
	Form("%f %% of data was not fMatched \n " , goodPercent) );
}
