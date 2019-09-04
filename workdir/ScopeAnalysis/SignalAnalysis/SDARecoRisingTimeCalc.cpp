#include "./SDARecoRisingTimeCalc.h"
#include "../JPetRecoSignalTools/JPetRecoSignalTools.h"
#include "../../../framework/JPetManager/JPetManager.h"

ClassImp(SDARecoRisingTimeCalc);


//standard constructor
SDARecoRisingTimeCalc::SDARecoRisingTimeCalc(const char* name, const char* title,
                   const char* in_file_suffix, const char* out_file_suffix)
                   : JPetCommonAnalysisModule(name, title, in_file_suffix, out_file_suffix)
{
  setVersion(MODULE_VERSION);
}

//no specific destructor needed
SDARecoRisingTimeCalc::~SDARecoRisingTimeCalc()
{
}

void SDARecoRisingTimeCalc::begin()
{
	const JPetParamBank& paramBank = getParamBank();
	fNumberOfPMTs = paramBank.getPMsSize();
	std::cout<<"Found " << fNumberOfPMTs << " PMTs in paramBank\n";

	for(unsigned int i = 0; i < fNumberOfPMTs; i++)
	{
		fIDs.push_back( paramBank.getPM(i).getID() );
		std::vector<double> k;
		fRiseTimes.push_back( k);
	}
	

        fFileName = fInFileName;

        size_t pos = fFileName.find(".root");

        fFileName = fFileName.substr(0, pos);


}

void SDARecoRisingTimeCalc::exec()
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
		  fRise= JPetRecoSignalTools::calculateRisingTime(signal);
		  fRiseTimes[j].push_back(fRise);
		  break;
		}
	}
		
  fEvent++;
  

}

void SDARecoRisingTimeCalc::end()
{

	TCanvas* c1 = new TCanvas();
	std::vector<double> minimums, maximums;
	
	//looking for max and min value for all offsets
	for ( unsigned int j = 0; j < fNumberOfPMTs; j++ )
	{
		minimums.push_back( JPetRecoSignalTools::min( fRiseTimes[j] ) );
		maximums.push_back( JPetRecoSignalTools::max( fRiseTimes[j] ) );
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
	bins = 125;
	if(bins<0)
		bins*=-1;

	bins = 50;
	
	//making vector of histos
	for( unsigned int j = 0; j < fNumberOfPMTs; j++ )
	{
		std::stringstream ss;
		ss << fIDs[j];
		std::string title = "Rise time for PMT" + ss.str();
		fRiseHistos.push_back(new TH1F( title.c_str(), title.c_str() , bins, 0, 10000 ));
         	 ss.str( std::string() );
	          ss.clear();
	}

	//filling histos
	for( unsigned int j = 0; j < fNumberOfPMTs; j++ )
	{	
		for(unsigned int i = 0; i < fRiseTimes[j].size(); ++i)
		{
			fRiseHistos[j]->Fill(fRiseTimes[j][i],1);
		}
	}

	//looking for tallest histo
	unsigned int tallest = 0;
	int tallestHeight = (fRiseHistos[0]->GetBinContent( fRiseHistos[0]->GetMaximumBin() ) );
	for(unsigned int j = 1; j < fNumberOfPMTs; j++)
	{
		if( fRiseHistos[j]->GetBinContent( fRiseHistos[j]->GetMaximumBin() ) > tallestHeight )
		{
			tallest = j;
			tallestHeight = fRiseHistos[j]->GetBinContent(fRiseHistos[j]->GetMaximumBin());
		}
	}
	
	
	tallest = 3;
	for( unsigned int j = 0; j < fNumberOfPMTs; j++)
	{
	       double peakPosition = fRiseHistos[j]->GetBinCenter( fRiseHistos[j]->GetMaximumBin() );
	       double range = peakPosition * 2;
	       std::stringstream ss;
		ss << fIDs[j];
		std::string title = "Risetime for PMT" + ss.str() +" rescaled range";
		fRiseHistosRescaled.push_back(new TH1F( title.c_str(), title.c_str() , bins, 0, range ));
         	ss.str( std::string() );
	        ss.clear();
		std::cout << peakPosition << "\t" << range <<std::endl;
	}

	//filling histos
	for( unsigned int j = 0; j < fNumberOfPMTs; j++ )
	{	
		for(unsigned int i = 0; i < fRiseTimes[j].size(); ++i)
		{
			fRiseHistosRescaled[j]->Fill(fRiseTimes[j][i],1);
		}
	}
	
	//plotting tallest histo
	fRiseHistosRescaled[tallest]->GetXaxis()->SetTitle("Rise time [ps]");
	fRiseHistosRescaled[tallest]->GetYaxis()->SetTitle("Counts");
	fRiseHistosRescaled[tallest]->Draw();
	fRiseHistosRescaled[tallest]->SetLineWidth(4);

	TLegend* legend = new TLegend(0.4,0.6,0.89,0.89);
        legend->AddEntry(fRiseHistosRescaled[tallest], fRiseHistosRescaled[tallest]->GetTitle(),"l");
	//plotting the rest
	for(unsigned int i = 0; i < fRiseHistosRescaled.size(); i++)
	{
		if(i == tallest)
			continue;
		fRiseHistosRescaled[i]->SetLineWidth(4);
		fRiseHistosRescaled[i]->SetLineColor(i+1);
// 		fRiseHistosRescaled[i]->Draw("same");
// 		legend->AddEntry(fRiseHistosRescaled[i], fRiseHistosRescaled[i]->GetTitle(),"l");
		
	}	

// 	legend->Draw();

        std::string title = fFileName + "_fRiseHistosForAll";

        c1->SaveAs( (title+".png").c_str() );
        c1->SaveAs( (title+".root").c_str() );

}

