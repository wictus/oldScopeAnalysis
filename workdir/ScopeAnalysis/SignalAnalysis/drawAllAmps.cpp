#include "./drawAllAmps.h"
#include "../JPetRecoSignalTools/JPetRecoSignalTools.h"
#include "../../../framework/JPetManager/JPetManager.h"

ClassImp(drawAllAmps);


//standard constructor
drawAllAmps::drawAllAmps(const char* name, const char* title,
                   const char* in_file_suffix, const char* out_file_suffix)
                   : JPetCommonAnalysisModule(name, title, in_file_suffix, out_file_suffix)
{
  setVersion(MODULE_VERSION);
}

//no specific destructor needed
drawAllAmps::~drawAllAmps()
{
}

void drawAllAmps::begin()
{
	const JPetParamBank& paramBank = getParamBank();
	fNumberOfPMTs = paramBank.getPMsSize();
	std::cout<<"Found " << fNumberOfPMTs << " PMTs in paramBank\n";

	for(unsigned int i = 0; i < fNumberOfPMTs; i++)
	{
		fIDs.push_back( paramBank.getPM(i).getID() );
		std::vector<double> k;
		fAmps.push_back( k);
	}
	

        fFileName = fInFileName;

        size_t pos = fFileName.find(".root");

        fFileName = fFileName.substr(0, pos);


}

void drawAllAmps::exec()
{
// Take next entry 
  fReader->getEntry(fEvent);

  // Cast data from the entry into JPetRecoSignal
  const JPetRecoSignal& signal = (JPetRecoSignal&) fReader->getData();

	for(unsigned int j = 0; j < fNumberOfPMTs; j++)
	{
		if( (signal.getPM().getID()) == fIDs[j] )
		{
		  //get offset and fill
		  fAmp= signal.getAmplitude();
		  fAmps[j].push_back(fAmp);
		  break;
		}
	}
		
  fEvent++;
  

}

void drawAllAmps::end()
{

	TCanvas* c1 = new TCanvas();
	std::vector<double> minimums, maximums;
	
	//looking for max and min value for all offsets
	for ( unsigned int j = 0; j < fNumberOfPMTs; j++ )
	{
		minimums.push_back( JPetRecoSignalTools::min( fAmps[j] ) );
		maximums.push_back( JPetRecoSignalTools::max( fAmps[j] ) );
	}	
		double maximum = JPetRecoSignalTools::max( maximums );
		double minimum = JPetRecoSignalTools::min( minimums );

		maximum = maximum + maximum*0.1;
		minimum = minimum - minimum*0.1;

// 	maximum = 150;
// 	minimum = 0;

	if(minimum < -100)
		minimum = -100;

	int bins = maximum - minimum;
	bins /= 20;
	bins = 75;
	if(bins<0)
		bins*=-1;

	//making vector of histos
	for( unsigned int j = 0; j < fNumberOfPMTs; j++ )
	{
		std::stringstream ss;
		ss << fIDs[j];
		std::string title = "Amp for PMT" + ss.str();
		fAmpHistos.push_back(new TH1F( title.c_str(), title.c_str() , bins, 0, 500 ));
         	 ss.str( std::string() );
	          ss.clear();
	}

	//filling histos
	for( unsigned int j = 0; j < fNumberOfPMTs; j++ )
	{	
		for(unsigned int i = 0; i < fAmps[j].size(); ++i)
		{
			fAmpHistos[j]->Fill(fAmps[j][i],1);
		}
	}

	//looking for tallest histo
	unsigned int tallest = 0;
	int tallestHeight = (fAmpHistos[0]->GetBinContent( fAmpHistos[0]->GetMaximumBin() ) );
	for(unsigned int j = 1; j < fNumberOfPMTs; j++)
	{
		if( fAmpHistos[j]->GetBinContent( fAmpHistos[j]->GetMaximumBin() ) > tallestHeight )
		{
			tallest = j;
			tallestHeight = fAmpHistos[j]->GetBinContent(fAmpHistos[j]->GetMaximumBin());
		}
	}

	//plotting tallest histo
	fAmpHistos[tallest]->GetXaxis()->SetTitle("Amplitude [mV]");
	fAmpHistos[tallest]->GetYaxis()->SetTitle("Counts");
	fAmpHistos[tallest]->Draw();
	fAmpHistos[tallest]->SetLineWidth(4);

	TLegend* legend = new TLegend(0.4,0.6,0.89,0.89);
        legend->AddEntry(fAmpHistos[tallest], fAmpHistos[tallest]->GetTitle(),"l");
	//plotting the rest
	for(unsigned int i = 0; i < fAmpHistos.size(); i++)
	{
		if(i == tallest)
			continue;
		if(fAmpHistos[i]->GetEntries() != 0 )
		{
		  fAmpHistos[i]->SetLineWidth(4);
		  fAmpHistos[i]->SetLineColor(i+1);
		  fAmpHistos[i]->Draw("same");
		  legend->AddEntry(fAmpHistos[i], fAmpHistos[i]->GetTitle(),"l");
		}
		
	}	

	legend->Draw();

        std::string title = fFileName + "_fAmpHistosForAll";

        c1->SaveAs( (title+".png").c_str() );
        c1->SaveAs( (title+".root").c_str() );

}

