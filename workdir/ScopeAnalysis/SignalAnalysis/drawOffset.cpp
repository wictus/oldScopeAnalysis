#include "./drawOffset.h"
#include "../JPetRecoSignalTools/JPetRecoSignalTools.h"
#include "../../../framework/JPetManager/JPetManager.h"
#include <algorithm> 


ClassImp(drawOffset);


//standard constructor
drawOffset::drawOffset(const char* name, const char* title,
                   const char* in_file_suffix, const char* out_file_suffix, const int PMTiD)
                   : JPetCommonAnalysisModule(name, title, in_file_suffix, out_file_suffix)
{
	fID = PMTiD;
  setVersion(MODULE_VERSION);
}

//no specific destructor needed
drawOffset::~drawOffset()
{
}

void drawOffset::begin()
{
	const JPetParamBank& paramBank = getParamBank();
	int numberOfPMTs = paramBank.getPMsSize();
	fFoundPMTinParam = false;	

	std::cout<<"Found " << numberOfPMTs << " PMTs in paramBank\n";

	for(int i = 0; i < numberOfPMTs; i++)
	{
		std::cout<<paramBank.getPM(i).getID()<<std::endl;
		if( paramBank.getPM(i).getID() == fID )
			fFoundPMTinParam = true;
	}
	
	if(!fFoundPMTinParam)
	{	
		std::cout<<"Did not find PMT id: " << fID << " in paramBank\n";
	}

}

void drawOffset::exec()
{
// Take next entry 
  fReader->getEntry(fEvent);

  // Cast data from the entry into JPetRecoSignal
  const JPetRecoSignal& signal = (JPetRecoSignal&) fReader->getData();

	if( (signal.getPM().getID()) == fID )
	{
	  //get fOffset and fill
	  fOffset = signal.getOffset();
	  fOffsets.push_back(fOffset);
	}

		

  fEvent++;
  

}

void drawOffset::end()
{
	TCanvas* c1 = new TCanvas();
	double minimum = JPetRecoSignalTools::min(fOffsets);
	minimum = minimum - minimum*0.1;

	double maximum = JPetRecoSignalTools::max(fOffsets);
	maximum = maximum + maximum*0.1;
	
	int bins = maximum - minimum;
	bins *= 100;

	if(bins<0)
		bins*=-1;

	fOffsetHisto = new TH1F("fOffset", "fOffset", bins, minimum, maximum );

	for(unsigned int i = 0; i < fOffsets.size(); ++i)
	{
		fOffsetHisto->Fill(fOffsets[i],1);
	}

	fOffsetHisto->Draw();
	std::stringstream ss;
	ss << fID;
	
	c1->SaveAs( ("fOffsetFor"+ss.str()+".png").c_str());
          ss.str( std::string() );
          ss.clear();

}

