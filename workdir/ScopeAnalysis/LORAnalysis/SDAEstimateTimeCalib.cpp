#include "./SDAEstimateTimeCalib.h"

SDAEstimateTimeCalib::SDAEstimateTimeCalib(const char* name, const char* title,
               const char* in_file_suffix, const char* out_file_suffix, std::map<int, double> thresholds) : JPetCommonAnalysisModule( name, title, in_file_suffix, out_file_suffix )
{
	fSelectedThreshold = thresholds;
}

SDAEstimateTimeCalib::~SDAEstimateTimeCalib(){}

void SDAEstimateTimeCalib::begin()
{
	const JPetParamBank& bank = getParamBank();

	for( int i = 0; i < bank.getScintillatorsSize(); i++)
	{
		std::vector<double> empty;
		deltaTPerScint[ bank.getScintillator(i).getID() ] = empty;
	}
}

void SDAEstimateTimeCalib::exec()
{
	fReader->getEntry(fEvent);

	const JPetLOR& fLOR = dynamic_cast< JPetLOR& > ( fReader->getData() );

	(deltaTPerScint[fLOR.getFirstHit().getSignalA().getRecoSignal().getPM().getScin().getID()]).push_back( fLOR.getFirstHit().getTimeDiff() );

	(deltaTPerScint[fLOR.getSecondHit().getSignalA().getRecoSignal().getPM().getScin().getID()]).push_back( fLOR.getSecondHit().getTimeDiff() );
	
	fEvent++;
}

void SDAEstimateTimeCalib::end()
{
  const JPetParamBank& bank = getParamBank();
	gStyle->SetOptFit(1);
	TCanvas* c1 = new TCanvas();
	TH1F* deltaT = new TH1F("timeDifference", "timeDifference", 200, -5, 5);
	
	for(int i = 0; i < bank.getScintillatorsSize(); i++)
	{
	  
	  for( unsigned int j = 0; j < deltaTPerScint[ bank.getScintillator(i).getID() ].size(); j++ )
	  {
		deltaT->Fill(deltaTPerScint[bank.getScintillator(i).getID()][j]/1000.0);
	  }
	  
	
	TString title = "Time difference for";
	title+= bank.getScintillator(i).getID() ;
	deltaT->SetTitle(title);
	deltaT->Sumw2();
	deltaT->Draw();
        deltaT->Fit("gaus","QI");

  std::string filePath = (std::string)fHeader->getBaseFileName();
  filePath = filePath.substr(filePath.find_last_of("/")+1 );
  filePath = filePath.substr(0, filePath.find_last_of(".") );
  double position = fHeader->getSourcePosition();

  std::stringstream buf;
  buf << position << "_" << fSelectedThreshold[ bank.getScintillator(i).getID() ] << "mV_" << bank.getScintillator(i).getID();

	c1->SaveAs( (filePath+"_"+buf.str()+".png").c_str() );
	buf.clear();
	buf.str(std::string());

	deltaT->Reset();
	}
}
