#include "./SDARecoEnergyResolutionFit.h"
#include "../JPetRecoSignalTools/JPetRecoSignalTools.h"
#include "../../../framework/JPetManager/JPetManager.h"

ClassImp(SDARecoEnergyResolutionFit);

inline bool fCheckPath(std::string path){ struct stat buffer; return (stat ( path.c_str(), &buffer) == 0); };

//standard constructor
SDARecoEnergyResolutionFit::SDARecoEnergyResolutionFit(const char* name, const char* title,
                   const char* in_file_suffix, const char* out_file_suffix)
                   : JPetCommonAnalysisModule(name, title, in_file_suffix, out_file_suffix)
{
  setVersion(MODULE_VERSION);
}

//no specific destructor needed
SDARecoEnergyResolutionFit::~SDARecoEnergyResolutionFit()
{
}

void SDARecoEnergyResolutionFit::begin()
{
	const JPetParamBank& paramBank = getParamBank();
	fNumberOfPMTs = paramBank.getPMsSize();
	std::cout<<"Found " << fNumberOfPMTs << " PMTs in paramBank\n";

	for(unsigned int i = 0; i < fNumberOfPMTs; i++)
	{
		fIDs.push_back( paramBank.getPM(i).getID() );
		std::vector<double> k;
		fCharges.push_back( k);
	}
	

        fFileName = fInFileName;

        size_t pos = fFileName.find(".root");

        fFileName = fFileName.substr(0, pos);

	
  bool resultFileExist = fCheckPath( "energyFit.txt" );
  std::ofstream resultsFile;
  
  if( !resultFileExist )
  {
    resultsFile.open("energyFit.txt");
    resultsFile << "Filename\t Constant:\tMean:\t Sigma:\t ChiSquare:\t NDF:\tChi2/NDF\t"<< std::endl; 
    resultsFile.close();
  }
}

void SDARecoEnergyResolutionFit::exec()
{
// Take next entry 
  fReader->getEntry(fEvent);

  // Cast data from the entry into JPetRecoSignal
  const JPetRecoSignal& signal = (JPetRecoSignal&) fReader->getData();

	for(unsigned int j = 0; j < fNumberOfPMTs; j++)
	{
		if( (signal.getPM().getID()) == 4 )
		{
		  //get offset and fill
		  fCharges[j].push_back( signal.getCharge() );
		  break;
		}
	}
		
  fEvent++;
  

}

void SDARecoEnergyResolutionFit::end()
{

	TCanvas* c1 = new TCanvas();
	std::vector<double> minimums, maximums;
	
	//looking for max and min value for all offsets
	for ( unsigned int j = 0; j < fNumberOfPMTs; j++ )
	{
		minimums.push_back( JPetRecoSignalTools::min( fCharges[j] ) );
		maximums.push_back( JPetRecoSignalTools::max( fCharges[j] ) );
	}	
		double maximum = JPetRecoSignalTools::max( maximums );
		double minimum = JPetRecoSignalTools::min( minimums );

		maximum = maximum + maximum*0.1;
		minimum = minimum - minimum*0.1;

	if(minimum < -100)
		minimum = -100;

	int bins = maximum - minimum;
	bins /= 20;
	bins = 500;
	if(bins<0)
		bins*=-1;

	//making vector of histos
	for( unsigned int j = 0; j < fNumberOfPMTs; j++ )
	{
		std::stringstream ss;
		ss << fIDs[j];
		std::string title = "Charge for PMT" + ss.str();
		fChargeHistos.push_back(new TH1F( title.c_str(), title.c_str() , bins, 0, 1000 ));
         	 ss.str( std::string() );
	          ss.clear();
	}

	//filling histos
	for( unsigned int j = 0; j < fNumberOfPMTs; j++ )
	{	
		for(unsigned int i = 0; i < fCharges[j].size(); ++i)
		{
			fChargeHistos[j]->Fill(fCharges[j][i],1);
		}
	}

	for( unsigned int j = 0; j < fNumberOfPMTs; j++)
	{
	       double peakPosition = fChargeHistos[j]->GetBinCenter( fChargeHistos[j]->GetMaximumBin() );
	       double range = peakPosition/660 * 1000;
	       std::stringstream ss;
		ss << fIDs[j];
		std::string title = "Charge for PMT" + ss.str() +" rescaled range";
		fChargeHistosRescaled.push_back(new TH1F( title.c_str(), title.c_str() , bins, 0, range ));
         	ss.str( std::string() );
	        ss.clear();
	}
	
	//filling histos
	for( unsigned int j = 0; j < fNumberOfPMTs; j++ )
	{	
		for(unsigned int i = 0; i < fCharges[j].size(); ++i)
		{
			fChargeHistosRescaled[j]->Fill(fCharges[j][i],1);
		}
	}

	
	//looking for tallest histo
	unsigned int tallest = 0;
	int tallestHeight = (fChargeHistosRescaled[0]->GetBinContent( fChargeHistosRescaled[0]->GetMaximumBin() ) );
	for(unsigned int j = 1; j < fNumberOfPMTs; j++)
	{
		if( fChargeHistosRescaled[j]->GetBinContent( fChargeHistosRescaled[j]->GetMaximumBin() ) > tallestHeight )
		{
			tallest = j;
			tallestHeight = fChargeHistosRescaled[j]->GetBinContent(fChargeHistosRescaled[j]->GetMaximumBin());
		}
	}
	
	
	std::ofstream resultsFile;
	resultsFile.open("energyFit.txt",std::ios_base::app);
	//plotting tallest histo
	fChargeHistosRescaled[tallest]->GetXaxis()->SetTitle("Charge [pC]");
	fChargeHistosRescaled[tallest]->GetYaxis()->SetTitle("Counts");
	fChargeHistosRescaled[tallest]->Draw();
	fChargeHistosRescaled[tallest]->SetLineWidth(4);
	double peakPosition = fChargeHistosRescaled[tallest]->GetBinCenter(fChargeHistosRescaled[tallest]->GetMaximumBin());
	TF1* onlyGaus = new TF1("onlyGaus", "gaus",  peakPosition*0.9, peakPosition*1.1);
	fChargeHistosRescaled[tallest]->Fit("onlyGaus","R");
	
	TF1* gausAndPoly = new TF1("gausAndPoly", "gaus(0)+pol2(3)", peakPosition*0.8, peakPosition*1.2);
	gausAndPoly->SetParameter(0, onlyGaus->GetParameter(0) );
	gausAndPoly->SetParameter(1, onlyGaus->GetParameter(1));
	gausAndPoly->SetParameter(2, onlyGaus->GetParameter(2));
	
	fChargeHistosRescaled[tallest]->Fit("gausAndPoly","R");
	
	
	
	TF1* twoGausesAndPoly = new TF1("twoGausesAndPoly", "gaus(0) + gaus(3) + pol2(6)", peakPosition*0.8, peakPosition*1.2);
	twoGausesAndPoly->SetParameter(0, onlyGaus->GetParameter(0));
	twoGausesAndPoly->SetParameter(1, onlyGaus->GetParameter(1));
	twoGausesAndPoly->SetParameter(2, onlyGaus->GetParameter(2));
	twoGausesAndPoly->SetParameter(3, onlyGaus->GetParameter(0)/5.0);
	twoGausesAndPoly->SetParameter(4, onlyGaus->GetParameter(1)*(1.0-32.0/660.0));
	twoGausesAndPoly->SetParameter(5, onlyGaus->GetParameter(3));
	twoGausesAndPoly->FixParameter(6, gausAndPoly->GetParameter(3));
	twoGausesAndPoly->FixParameter(7, gausAndPoly->GetParameter(4));
	twoGausesAndPoly->FixParameter(8, gausAndPoly->GetParameter(5));
// 	fChargeHistos[tallest]->Fit("twoGausesAndPoly","R");
	
	TF1* justSecondGaus = new TF1("justSecondGaus", "gaus", peakPosition*0.8, peakPosition*1.2);
	justSecondGaus->SetParameter(0, twoGausesAndPoly->GetParameter(3) );
	justSecondGaus->SetParameter(1, twoGausesAndPoly->GetParameter(4) );
	justSecondGaus->SetParameter(2, twoGausesAndPoly->GetParameter(5) );
	justSecondGaus->SetLineColor(24);
// 	justSecondGaus->Draw("same");
		
	std::string name = fFileName.substr( fFileName.find_last_of("V") - 4, 4 );
	
	resultsFile << name << "\t" <<  gausAndPoly->GetParameter(0) <<" +/ " << gausAndPoly->GetParError(0) << "\t" << gausAndPoly->GetParameter(1) <<" +/ " << gausAndPoly->GetParError(1)<< "\t" << gausAndPoly->GetParameter(2)<<" +/ " << gausAndPoly->GetParError(2) << "\t" << gausAndPoly->GetChisquare() << "\t" << gausAndPoly->GetNDF() << "\t" << (double)gausAndPoly->GetChisquare() / (double)gausAndPoly->GetNDF() << std::endl;
	
// 	resultsFile << fFileName << "\t" <<  twoGausesAndPoly->GetParameter(0) <<" +/ " << twoGausesAndPoly->GetParError(0) << "\t" << twoGausesAndPoly->GetParameter(1) <<" +/ " << twoGausesAndPoly->GetParError(1)<< "\t" << twoGausesAndPoly->GetParameter(2)<<" +/ " << twoGausesAndPoly->GetParError(2) << "\t" << twoGausesAndPoly->GetChisquare() << "\t" << twoGausesAndPoly->GetNDF() << "\t" << (double)twoGausesAndPoly->GetChisquare() / (double)twoGausesAndPoly->GetNDF() << std::endl;
	
	resultsFile.close();

// 	TLegend* legend = new TLegend(0.4,0.6,0.89,0.89);
//         legend->AddEntry(fChargeHistosRescaled[tallest], fChargeHistosRescaled[tallest]->GetTitle(),"l");
	
	/*
	//plotting the rest
	for(unsigned int i = 0; i < fChargeHistos.size(); i++)
	{
		if(i == tallest)
			continue;
		fChargeHistos[i]->SetLineWidth(4);
		fChargeHistos[i]->SetLineColor(i+1);
		fChargeHistos[i]->Draw("same");
		legend->AddEntry(fChargeHistos[i], fChargeHistos[i]->GetTitle(),"l");
		
	}	
	legend->Draw();

	*/
	
        std::string title = fFileName + "_energyFit";

        c1->SaveAs( (title+".png").c_str() );
        c1->SaveAs( (title+".root").c_str() );

	for( unsigned int j = 0; j < fNumberOfPMTs; j++ )
	{	
		delete fChargeHistos[j];
	}
}

