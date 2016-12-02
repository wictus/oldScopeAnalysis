#include "./drawAllOffsets.h"
#include "../JPetRecoSignalTools/JPetRecoSignalTools.h"
#include "../../../framework/JPetManager/JPetManager.h"

ClassImp(drawAllOffsets);


//standard constructor
drawAllOffsets::drawAllOffsets(const char* name, const char* title,
                   const char* in_file_suffix, const char* out_file_suffix)
                   : JPetCommonAnalysisModule(name, title, in_file_suffix, out_file_suffix)
{
  setVersion(MODULE_VERSION);

}

//no specific destructor needed
drawAllOffsets::~drawAllOffsets()
{
}

void drawAllOffsets::begin()
{
	const JPetParamBank& paramBank = getParamBank();
	fNumberOfPMTs = paramBank.getPMsSize();
	std::cout<<"Found " << fNumberOfPMTs << " PMTs in paramBank\n";

	for(unsigned int i = 0; i < fNumberOfPMTs; i++)
	{
		fIDs.push_back( paramBank.getPM(i).getID() );
		std::vector<double> k;
		fOffsets.push_back( k);
	}
	
	fFileName = fInFileName;

	size_t pos = fFileName.find(".root");
	
	fFileName = fFileName.substr(0, pos);

}

void drawAllOffsets::exec()
{
// Take next entry 
  fReader->getEntry(fEvent);

  // Cast data from the entry into JPetRecoSignal
  const JPetRecoSignal& signal = (JPetRecoSignal&) fReader->getData();

	for(unsigned int j = 0; j < fNumberOfPMTs; j++)
	{
		if( (signal.getPM().getID()) == fIDs[j] )
		{
		  //get fOffset and fill
		  fOffset = signal.getOffset();
		  fOffsets[j].push_back(fOffset);
		  break;
		}
	}
		
  fEvent++;
  

}

void drawAllOffsets::end()
{

	TCanvas* c1 = new TCanvas();
	std::vector<double> minimums, maximums;
	
	//looking for max and min value for all fOffsets
	for (unsigned  int j = 0; j < fNumberOfPMTs; j++ )
	{
		minimums.push_back( JPetRecoSignalTools::min( fOffsets[j] ) );
		maximums.push_back( JPetRecoSignalTools::max(fOffsets[j] ) );
	}	
		double maximum = JPetRecoSignalTools::max( maximums );
		double minimum = JPetRecoSignalTools::min( minimums );

		maximum = maximum + maximum*0.1;
		minimum = minimum - minimum*0.1;

	if(maximum > 100)
		maximum =100;
	if(minimum < -100)
		minimum = -100;

	int bins = maximum - minimum;
	bins *= 10;

	if(bins<0)
		bins*=-1;

	//making vector of histos
	for(unsigned  int j = 0; j < fNumberOfPMTs; j++ )
	{
		std::stringstream ss;
		ss << fIDs[j];
		std::string title = "Offset for PMT" + ss.str();
		fOffsetHistos.push_back(new TH1F( title.c_str(), title.c_str() , bins, minimum, maximum ));
         	 ss.str( std::string() );
	          ss.clear();
	}

	//filling histos
	for(unsigned  int j = 0; j < fNumberOfPMTs; j++ )
	{	
		for(unsigned int i = 0; i < fOffsets[j].size(); ++i)
		{
			fOffsetHistos[j]->Fill(fOffsets[j][i],1);
		}
	}

	//looking for tallest histo
	unsigned int tallest = 0;
	int tallestHeight = (fOffsetHistos[0]->GetBinContent( fOffsetHistos[0]->GetMaximumBin() ) );
	for(unsigned int j = 1; j < fNumberOfPMTs; j++)
	{
		if( fOffsetHistos[j]->GetBinContent( fOffsetHistos[j]->GetMaximumBin() ) > tallestHeight )
		{
			tallest = j;
			tallestHeight = fOffsetHistos[j]->GetBinContent(fOffsetHistos[j]->GetMaximumBin());
		}
	}

	//plotting tallest histo
	fOffsetHistos[tallest]->GetXaxis()->SetTitle("Offset [mV]");
	fOffsetHistos[tallest]->GetYaxis()->SetTitle("Counts");
	fOffsetHistos[tallest]->SetLineWidth(4);
	fOffsetHistos[tallest]->Draw();
	//plotting the rest
	TLegend* legend = new TLegend(0.4,0.6,0.89,0.89);
	legend->AddEntry(fOffsetHistos[tallest], fOffsetHistos[tallest]->GetTitle(),"l");
	for(unsigned int i = 0; i < fOffsetHistos.size(); i++)
	{
		if(i == tallest)
			continue;
		fOffsetHistos[i]->SetLineWidth(4);
		fOffsetHistos[i]->SetLineColor(i+1);
		fOffsetHistos[i]->Draw("same");
		legend->AddEntry(fOffsetHistos[i], fOffsetHistos[i]->GetTitle(),"l");
		
	}	

	legend->Draw();
	
	std::string title = fFileName + "_fOffsetsForAll.root";
	
	c1->SaveAs( title.c_str() );

}

