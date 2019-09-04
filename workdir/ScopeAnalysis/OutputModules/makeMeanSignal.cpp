#include "./makeMeanSignal.h"
#include "../JPetRecoSignalTools/JPetRecoSignalTools.h"
#include "../../../framework/JPetManager/JPetManager.h"

ClassImp(makeMeanSignal);


//standard constructor
makeMeanSignal::makeMeanSignal(const char* name, const char* title,
                   const char* in_file_suffix, const char* out_file_suffix, int PMTID)
                   : JPetCommonAnalysisModule(name, title, in_file_suffix, out_file_suffix), fPMTID(PMTID)
{
  setVersion(MODULE_VERSION);
}

//no specific destructor needed
makeMeanSignal::~makeMeanSignal()
{
}

void makeMeanSignal::begin()
{
}

void makeMeanSignal::exec()
{
// Take next entry 
  fReader->getEntry(fEvent);

  // Cast data from the entry into JPetRecoSignal
  const JPetRecoSignal& signal = (JPetRecoSignal&) fReader->getData();

  if( (signal.getPM().getID()) == fPMTID )
    signals.push_back( signal );
		
  fEvent++;
  

}

void makeMeanSignal::end()
{
  std::vector<double> meanPoints;
  std::vector<double> timeStamps;
  std::vector< shapePoint > signalPoints = signals[0].getShape();
  double referenceTime = JPetRecoSignalTools::calculateTimeAtThreshold( signals[0], 100 )/1000;
  for(int p = 0; p < signalPoints.size(); p++)
  {
     meanPoints.push_back(signalPoints[p].amplitude);
     timeStamps.push_back(signalPoints[p].time/1000) ;
  }
//   int indexOfReference = JPetRecoSignalTools::findIndexAtValue(referenceTime, timeStamps);
  signalPoints.clear();
//   JPetRecoSignalTools::savePNGOfBadSignal( signals[0], 0);
   for( int s = 1; s < signals.size(); s++)
   {
     
      signalPoints = signals[s].getShape();
      for(unsigned int p = 0; p < signalPoints.size(); p++)
      {
	std::cout << timeStamps[p] << "\t" << signalPoints[p].time / 1000 <<std::endl;
// 	std::cout << timeStamps[p] << "\t" << signalPoints[p].time / 1000 - referenceTime/1000 - timeStamps[0] + signalPoints[0].time / 1000 <<std::endl;
      }
      signalPoints.clear();

   }
//    for( int s = 1; s < signals.size(); s++)
//    {
// //      JPetRecoSignalTools::savePNGOfBadSignal( signals[s], s);
//      signalPoints = signals[s].getShape();
//      for(unsigned int p = 0; p < signalPoints.size(); p++)
//        meanPoints[p] = (meanPoints[p]+signalPoints[p].amplitude) / 2 ;
//      signalPoints.clear();
//   }
  std::vector<double> tAtThr;
  TH1F* histoTest = new TH1F("histo", "histo", 1000, -30, 30);
  
  
  for(int s = 0; s < signals.size(); s++)
  {
    histoTest->Fill( JPetRecoSignalTools::calculateTimeAtThreshold( signals[s], 100 )/1000 );
//     std::cout << JPetRecoSignalTools::calculateTimeAtThreshold( signals[s], 10 ) << std::endl;
  }
  histoTest->SaveAs("kiko.root");
  
//   TGraph* meanSignal = new TGraph( meanPoints.size(), &timeStamps[0], &meanPoints[0] );
//   meanSignal->SetTitle("");
//   meanSignal->GetXaxis()->SetTitle("Time [ns]");
//   meanSignal->GetYaxis()->SetTitle("Amplitude [mV]");
//   meanSignal->SetMarkerStyle(21);
//   meanSignal->SetMarkerSize(0.5);
//   meanSignal->Draw("AP");
//   TString finalName = "meanSignalsSABAT/";
//   finalName += fHeader->getBaseFileName();
//   finalName = finalName(finalName.Last('/')+1, finalName.Length()-1);
//   finalName = finalName(0, finalName.Last('.'));
//   finalName+= fHeader->getSourcePosition();

  
  
//   finalName+=".root";
//   meanSignal->SaveAs( finalName );

}

