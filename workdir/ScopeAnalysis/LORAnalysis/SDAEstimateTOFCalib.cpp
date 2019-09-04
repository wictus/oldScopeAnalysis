#include "./SDAEstimateTOFCalib.h"

SDAEstimateTOFCalib::SDAEstimateTOFCalib(const char* name, const char* title,
               const char* in_file_suffix, const char* out_file_suffix, const double threshold) : JPetCommonAnalysisModule( name, title, in_file_suffix, out_file_suffix )
{
	fSelectedThreshold = threshold;
}

SDAEstimateTOFCalib::~SDAEstimateTOFCalib(){}

void SDAEstimateTOFCalib::begin()
{
}

void SDAEstimateTOFCalib::exec()
{
	fReader->getEntry(fEvent);

	const JPetLOR& fLOR = dynamic_cast< JPetLOR& > ( fReader->getData() );

	fTOFs.push_back( ( fLOR.getSecondHit().getTime() - fLOR.getFirstHit().getTime() ) );
	
	fEvent++;
}

void SDAEstimateTOFCalib::end()
{
	gStyle->SetOptFit(1);
	TCanvas* c1 = new TCanvas();
	TH1F* TOF = new TH1F("TOF", "TOF", 2000, -10, 10);

	for( unsigned int i = 0; i < fTOFs.size(); i++)
	{
		TOF->Fill(fTOFs[i]/1000.0);
	}

	  TOF->Sumw2();
	TOF->Draw();
        TOF->Fit("gaus","QI");

	c1->SaveAs("TOF.png");


}
