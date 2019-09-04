#include "./SDARecoFallingTimeCalc.h"
#include "../JPetRecoSignalTools/JPetRecoSignalTools.h"
#include "../../../framework/JPetManager/JPetManager.h"

ClassImp(SDARecoFallingTimeCalc);


//standard constructor
SDARecoFallingTimeCalc::SDARecoFallingTimeCalc(const char* name, const char* title,
                   const char* in_file_suffix, const char* out_file_suffix)
                   : JPetCommonAnalysisModule(name, title, in_file_suffix, out_file_suffix)
{
  setVersion(MODULE_VERSION);
}

//no specific destructor needed
SDARecoFallingTimeCalc::~SDARecoFallingTimeCalc()
{
}

void SDARecoFallingTimeCalc::begin()
{
	const JPetParamBank& paramBank = getParamBank();
	fNumberOfPMTs = paramBank.getPMsSize();
	std::cout<<"Found " << fNumberOfPMTs << " PMTs in paramBank\n";

	for(unsigned int i = 0; i < fNumberOfPMTs; i++)
	{
		fIDs.push_back( paramBank.getPM(i).getID() );
		std::vector<double> k;
		fFallTimes.push_back( k);
	}
	

        fFileName = fInFileName;

        size_t pos = fFileName.find(".root");

        fFileName = fFileName.substr(0, pos);


}

void SDARecoFallingTimeCalc::exec()
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
		  fFall= JPetRecoSignalTools::calculateFallingTime(signal);
		  fFallTimes[j].push_back(fFall);
		  break;
		}
	}
		
  fEvent++;
  

}

void SDARecoFallingTimeCalc::end()
{

	TCanvas* c1 = new TCanvas();
	std::vector<double> minimums, maximums;
	
	//looking for max and min value for all offsets
	for ( unsigned int j = 0; j < fNumberOfPMTs; j++ )
	{
		minimums.push_back( JPetRecoSignalTools::min( fFallTimes[j] ) );
		maximums.push_back( JPetRecoSignalTools::max( fFallTimes[j] ) );
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
	bins = 70;
	if(bins<0)
		bins*=-1;

	//making vector of histos
	for( unsigned int j = 0; j < fNumberOfPMTs; j++ )
	{
		std::stringstream ss;
		ss << fIDs[j];
		std::string title = "Fall time for PMT" + ss.str();
		fFallHistos.push_back(new TH1F( title.c_str(), title.c_str() , bins, 0, 300000 ));
         	 ss.str( std::string() );
	          ss.clear();
	}

	//filling histos
	for( unsigned int j = 0; j < fNumberOfPMTs; j++ )
	{	
		for(unsigned int i = 0; i < fFallTimes[j].size(); ++i)
		{
			fFallHistos[j]->Fill(fFallTimes[j][i],1);
		}
	}

	//looking for tallest histo
	unsigned int tallest = 0;
	int tallestHeight = (fFallHistos[0]->GetBinContent( fFallHistos[0]->GetMaximumBin() ) );
	for(unsigned int j = 1; j < fNumberOfPMTs; j++)
	{
		if( fFallHistos[j]->GetBinContent( fFallHistos[j]->GetMaximumBin() ) > tallestHeight )
		{
			tallest = j;
			tallestHeight = fFallHistos[j]->GetBinContent(fFallHistos[j]->GetMaximumBin());
		}
	}
	tallest = 3;
	for( unsigned int j = 0; j < fNumberOfPMTs; j++)
	{
	       double peakPosition = fFallHistos[j]->GetBinCenter( fFallHistos[j]->GetMaximumBin() );
	       double range = peakPosition * 2;
	       std::stringstream ss;
		ss << fIDs[j];
		std::string title = "Falltime for PMT" + ss.str() +" rescaled range";
		fFallHistosRescaled.push_back(new TH1F( title.c_str(), title.c_str() , bins, 0, range ));
         	ss.str( std::string() );
	        ss.clear();
	}
	
	for( unsigned int j = 0; j < fNumberOfPMTs; j++ )
	{	
		for(unsigned int i = 0; i < fFallTimes[j].size(); ++i)
		{
			fFallHistosRescaled[j]->Fill(fFallTimes[j][i],1);
		}
	}
	
	//plotting tallest histo
	
	fFallHistosRescaled[tallest]->GetXaxis()->SetTitle("Fall time [ps]");
	fFallHistosRescaled[tallest]->GetYaxis()->SetTitle("Counts");
	fFallHistosRescaled[tallest]->Draw();
	fFallHistosRescaled[tallest]->SetLineWidth(4);

	TLegend* legend = new TLegend(0.4,0.6,0.89,0.89);
        legend->AddEntry(fFallHistosRescaled[tallest], fFallHistosRescaled[tallest]->GetTitle(),"l");
	//plotting the rest
	for(unsigned int i = 0; i < fFallHistosRescaled.size(); i++)
	{
		if(i == tallest)
			continue;
		fFallHistosRescaled[i]->SetLineWidth(4);
		fFallHistosRescaled[i]->SetLineColor(i+1);
// 		fFallHistosRescaled[i]->Draw("same");
// 		legend->AddEntry(fFallHistosRescaled[i], fFallHistosRescaled[i]->GetTitle(),"l");
		
	}	

// 	legend->Draw();

        std::string title = fFileName + "_fFallHistosForAll";

        c1->SaveAs( (title+".png").c_str() );
        c1->SaveAs( (title+".root").c_str() );

}

