#include "SDALORCalculateWalkCorrection.h"
#include <boost/concept_check.hpp>

ClassImp(SDALORCalculateWalkCorrection);

SDALORCalculateWalkCorrection::SDALORCalculateWalkCorrection(const char* name, const char* title, const char* in_file_suffix, const char* out_file_suffix, std::map<int, double> thresholds): JPetCommonAnalysisModule(name, title, in_file_suffix, out_file_suffix)
{
  fThresholds = thresholds;
  gStyle->SetOptFit(1);
}

SDALORCalculateWalkCorrection::~SDALORCalculateWalkCorrection()
{

}

void SDALORCalculateWalkCorrection::begin()
{
  //prepare structure to contain corrections and data from strips
  //find out how many strips are there and make as many structures for coefficients storage
  
  const JPetParamBank& bank = getParamBank();
  
  for( int i = 0; i < bank.getPMsSize(); i++)
  {
    walkCoefficients dummy;
    dummy.scintID = bank.getPM(i).getID();
    dummy.alpha = 0;
    dummy.beta = 0;
    coefficients.push_back( dummy );
    std::vector<std::pair<double, double> > dummyVec;   
    points[ bank.getPM(i).getID() ] = dummyVec;
  }

  std::cout << "Coefficients to be filled " << coefficients.size() << " dummies \n";
   
}

void SDALORCalculateWalkCorrection::end()
{
    //doWalkCorrectionForEachPMTIndividually();
    doWalkForTimeDifference();
    doWalkForTOF();
}

void SDALORCalculateWalkCorrection::doWalkForTOF()
{
    TCanvas* c = new TCanvas();
    TH2F* histo = new TH2F("","", 100, -0.3, 0.3, 100, -1000, 1000);
    TH1F* deltaT = new TH1F("","", 500, -1000, 1000);
    std::vector<double> times, chargeSquareInverted;
    TString filePath = (std::string)fHeader->getBaseFileName();
    	filePath = filePath(filePath.Last('/')+1, filePath.Length()-1);
        filePath = filePath(0, filePath.Last('.'));
	std::stringstream buf;
	buf << fHeader->getSourcePosition();
	filePath+= buf.str();
	
	
    for(unsigned int j = 0; j < fTOFs.size(); j++)
    {
      
      double time = (fTOFs[j].first);
      double chargeSquareInvert =  fTOFs[j].second  ;
	
      histo->Fill( chargeSquareInvert, time );
      deltaT->Fill(time);
      times.push_back(time);
      chargeSquareInverted.push_back(chargeSquareInvert);	
    }

    histo->Draw("colz");
    c->SaveAs( filePath+"walkBefore.png" ) ;

    
    TGraph* graph = new TGraph( fTOFs.size(), &chargeSquareInverted[0], &times[0]);
    graph->Draw("AP");
    TF1* line = new TF1("line","[0]+x*[1]", -0.3, 0.3);
    graph->Fit("line");
    graph->GetXaxis()->SetTitle("1/sqrt(TOT) [1/sqrt(ps)]");
    graph->GetYaxis()->SetTitle("Time at threshold [ps]");
    graph->Draw("AP");
    c->SaveAs( filePath+"walkGraphBefore.png" ) ;

    deltaT->Fit("gaus");
    deltaT->Draw();
    c->SaveAs( filePath+"TOFBefore.png" );
    
    histo->Reset();
    deltaT->Reset();
    times.clear();
    chargeSquareInverted.clear();
    
    for(unsigned int j = 0; j < fTOFs.size(); j++)
    {
      double time = fTOFs[j].first - fTOFs[j].second *(double)line->GetParameter(1);
      double chargeSquareInvert =  fTOFs[j].second  ;
	
      histo->Fill( chargeSquareInvert, time );
      deltaT->Fill(time);
      times.push_back(time);
      chargeSquareInverted.push_back(chargeSquareInvert);	
    }
    
    histo->Draw("colz");
    c->SaveAs( filePath+"walkAfter.png" );
    
    graph = new TGraph( fTOFs.size() , &chargeSquareInverted[0], &times[0]);
    graph->Draw("AP");
    c->SaveAs( filePath+"walkGraphAfter.png" );
    
    deltaT->Fit("gaus");
    deltaT->Draw();
    c->SaveAs( filePath+"TOFAfter.png" );
    
    std::ofstream resultFile;
    resultFile.open("walkResults.txt", std::ios::app);
    resultFile << "file:\t for TOF" << filePath;
    resultFile << "\tconstant:\t" << line->GetParameter(0) <<"\t+/-\t" << line->GetParError(0);
    resultFile << "\talpha:\t" << line->GetParameter(1) <<"\t+/-\t" << line->GetParError(1) << std::endl;
    resultFile.close();
    
    delete graph;
    delete deltaT;
    delete histo;
    delete c;
}


void SDALORCalculateWalkCorrection::doWalkForTimeDifference()
{
  const JPetParamBank& bank = getParamBank();  
  for( int i = 0; i < bank.getScintillatorsSize(); i++)
  {
    TCanvas* c = new TCanvas();
    TH2F* histo = new TH2F("","", 50, -0.1, 0.4, 400, -10000, 10000);
    TH1F* deltaT = new TH1F("","", 400, -10000, 10000);
    std::vector<double> times, chargeSquareInverted;
    
    for(unsigned int j = 0; j < points[ bank.getScintillator(i).getID() ].size(); j++)
    {
      
      double time = points[ bank.getScintillator(i).getID() ][j].first;
      double chargeSquareInvert =  points[ bank.getScintillator(i).getID() ][j].second  ;
	
      histo->Fill( chargeSquareInvert, time );
      deltaT->Fill(time);
      times.push_back(time);
      chargeSquareInverted.push_back(chargeSquareInvert);
    }
    std::stringstream buf;
    buf << bank.getScintillator(i).getID();
    histo->GetYaxis()->SetRangeUser( histo->GetMean(2) - 1000, histo->GetMean(2) + 1000);
    histo->Draw("colz");
    
    TGraph* graph = new TGraph(points[bank.getScintillator(i).getID()].size(), &chargeSquareInverted[0], &times[0]);
    graph->Draw("AP");
    TF1* line = new TF1("line","[0]+x*[1]", 0, 1);
    graph->Fit("line");
    graph->GetXaxis()->SetTitle("1/#sqrt{pC} [1/#sqrt{ps}]");
    graph->GetYaxis()->SetTitle("Time at threshold [ps]");
    graph->Draw("AP");
    c->SaveAs( (buf.str()+"graphBefore.png").c_str() );

    deltaT->GetXaxis()->SetRangeUser(deltaT->GetMean() - 2000, deltaT->GetMean() + 2000);
    deltaT->Fit("gaus");
    deltaT->Draw();
    c->SaveAs( (buf.str()+"deltaTBefore.png").c_str() );
    
    histo->Draw("colz");
    line->Draw("same");
    c->SaveAs( (buf.str()+"Before.png").c_str() );
    TString filePath = (std::string)fHeader->getBaseFileName();
    	filePath = filePath(filePath.Last('/')+1, filePath.Length()-1);
        filePath = filePath(0, filePath.Last('.'));
	std::stringstream buf1;
	buf1 << fHeader->getSourcePosition();
	filePath+= buf1.str();

    std::ofstream resultFile;
    resultFile.open("walkResults.txt", std::ios::app);
    resultFile << "file:\t for DT \t" << filePath << "\t for scint " << bank.getScintillator(i).getID();
    resultFile << "\tconstant:\t" << line->GetParameter(0) <<"\t+/-\t" << line->GetParError(0);
    resultFile << "\talpha:\t" << line->GetParameter(1) <<"\t+/-\t" << line->GetParError(1) << std::endl;
    resultFile.close();
    histo->Reset();
    deltaT->Reset();
    times.clear();
    chargeSquareInverted.clear();
    
    for(unsigned int j = 0; j < points[ bank.getScintillator(i).getID() ].size(); j++)
    {
      double time = points[ bank.getScintillator(i).getID() ][j].first - (points[ bank.getScintillator(i).getID() ][j].second) *(double)line->GetParameter(1);
      double chargeSquareInvert = points[ bank.getScintillator(i).getID() ][j].second;
	
      histo->Fill( chargeSquareInvert, time);
      deltaT->Fill(time);
      times.push_back(time);
      chargeSquareInverted.push_back(chargeSquareInvert);
    }
    
    histo->GetYaxis()->SetRangeUser( histo->GetMean(2) - 1000, histo->GetMean(2) + 1000);
    histo->Draw("colz");
    c->SaveAs( (buf.str()+"After.root").c_str() );
    
    graph = new TGraph(points[bank.getScintillator(i).getID()].size(), &chargeSquareInverted[0], &times[0]);
    graph->Draw("AP");
    c->SaveAs( (buf.str()+"graphAfter.png").c_str() );
    
    deltaT->GetXaxis()->SetRangeUser(deltaT->GetMean() - 2000 , deltaT->GetMean() + 2000 );
    deltaT->Fit("gaus");
    deltaT->Draw();
    c->SaveAs( (buf.str()+"deltaTAfter.png").c_str() );
    buf.str(std::string() );
        
    histo->Reset();
    deltaT->Reset();
    times.clear();
    chargeSquareInverted.clear();
       delete graph;
    delete deltaT;
    delete histo;
    delete c;
  }
}


void SDALORCalculateWalkCorrection::doWalkCorrectionForEachPMTIndividually()
{
  TH1F* timeDiffAfterCorrection = new TH1F("","", 100, -1000, 1000);
  TCanvas* g = new TCanvas();
  std::vector< std::vector<double> > timesCorrected;
  const JPetParamBank& bank = getParamBank();
  // do plot and find coeeficients, save them in container
  for( int i = 0; i < bank.getPMsSize(); i++)
  {
    TCanvas* c = new TCanvas();
    TH2F* histo = new TH2F("","", 50, -0.1, 0.4, 400, -10000, 10000);
    TH1F* deltaT = new TH1F("","", 400, -10000, 10000);
    
    std::vector<double> times, chargeSquareInverted;
    for(unsigned int j = 0; j < points[ bank.getPM(i).getID() ].size(); j++)
    {
      
      double time = points[ bank.getPM(i).getID() ][j].first;
      double chargeSquareInvert =  1.0/sqrt( points[ bank.getPM(i).getID() ][j].second ) ;
	
      histo->Fill( chargeSquareInvert, time );
      deltaT->Fill(time);
      times.push_back(time);
      chargeSquareInverted.push_back(chargeSquareInvert);
    }
    
    
    std::stringstream buf;
    buf << bank.getPM(i).getID();
    histo->GetYaxis()->SetRangeUser( histo->GetMean(2) - 1000, histo->GetMean(2) + 1000);
    histo->Draw("colz");
    
    TGraph* graph = new TGraph(points[bank.getPM(0).getID()].size(), &chargeSquareInverted[0], &times[0]);
    graph->Draw("AP");
    TF1* line = new TF1("line","[0]+x*[1]", 0, 1);
    graph->Fit("line");
    graph->GetXaxis()->SetTitle("1/sqrt(Charge) [1/sqrt(pC)]");
    graph->GetYaxis()->SetTitle("Time at threshold [ps]");
    graph->Draw("AP");
    c->SaveAs( (buf.str()+"graphBefore.png").c_str() );

    deltaT->GetXaxis()->SetRangeUser(deltaT->GetMean() - 2000, deltaT->GetMean() + 2000);
    deltaT->Fit("gaus");
    deltaT->Draw();
    c->SaveAs( (buf.str()+"deltaTBefore.png").c_str() );
    
    histo->Draw("colz");
    line->Draw("same");
    c->SaveAs( (buf.str()+"Before.png").c_str() );
    
    histo->Reset();
    deltaT->Reset();
    times.clear();
    chargeSquareInverted.clear();
    
    for(unsigned int j = 0; j < points[ bank.getPM(i).getID() ].size(); j++)
    {
      double time = points[ bank.getPM(i).getID() ][j].first - 1.0/sqrt( points[ bank.getPM(i).getID() ][j].second ) *(double)line->GetParameter(1);
      double chargeSquareInvert =  1.0/sqrt( points[ bank.getPM(i).getID() ][j].second );
	
      histo->Fill( chargeSquareInvert, time);
      deltaT->Fill(time);
      times.push_back(time);
      chargeSquareInverted.push_back(chargeSquareInvert);
    }
    
    histo->GetYaxis()->SetRangeUser( histo->GetMean(2) - 1000, histo->GetMean(2) + 1000);
    histo->Draw("colz");
    c->SaveAs( (buf.str()+"After.png").c_str() );
    
    graph = new TGraph(points[bank.getPM(0).getID()].size(), &chargeSquareInverted[0], &times[0]);
    graph->Draw("AP");
    c->SaveAs( (buf.str()+"graphAfter.png").c_str() );
    
    deltaT->GetXaxis()->SetRangeUser(deltaT->GetMean() - 2000 , deltaT->GetMean() + 2000 );
    deltaT->Fit("gaus");
    deltaT->Draw();
    c->SaveAs( (buf.str()+"deltaTAfter.png").c_str() );
    
    buf.str(std::string() );
    
    timesCorrected.push_back(times);
    
    delete deltaT;
    delete histo;
    delete c;
  }
  
  for(unsigned int i = 0; i < timesCorrected[0].size(); i++ )
  {
      timeDiffAfterCorrection->Fill(timesCorrected[0][i] - timesCorrected[1][i]);
  }
  
  timeDiffAfterCorrection->Draw();
  timeDiffAfterCorrection->Fit("gaus");
  g->SaveAs("test.png");
  delete g;
  delete timeDiffAfterCorrection;
  
}

void SDALORCalculateWalkCorrection::prepareTimesForPMTCorrection(JPetLOR& fLOR)
{
  double threshold = fThresholds[ fLOR.getFirstHit().getSignalA().getRecoSignal().getPM().getScin().getID() ];
  std::pair<double,double> topLeft= std::make_pair(fLOR.getFirstHit().getSignalA().getRecoSignal().getRecoTimeAtThreshold(threshold), fLOR.getFirstHit().getSignalA().getRecoSignal().getCharge());
  std::pair<double,double> topRight= std::make_pair(fLOR.getFirstHit().getSignalB().getRecoSignal().getRecoTimeAtThreshold(threshold), fLOR.getFirstHit().getSignalB().getRecoSignal().getCharge());
  threshold = fThresholds[ fLOR.getSecondHit().getSignalA().getRecoSignal().getPM().getScin().getID() ];
  std::pair<double,double> bottomLeft= std::make_pair(fLOR.getSecondHit().getSignalA().getRecoSignal().getRecoTimeAtThreshold(threshold), fLOR.getSecondHit().getSignalA().getRecoSignal().getCharge());
  std::pair<double,double> bottomRight= std::make_pair(fLOR.getSecondHit().getSignalB().getRecoSignal().getRecoTimeAtThreshold(threshold), fLOR.getSecondHit().getSignalB().getRecoSignal().getCharge());
  
  
  (points[ fLOR.getFirstHit().getSignalA().getRecoSignal().getPM().getID() ]).push_back( topLeft );
  (points[ fLOR.getFirstHit().getSignalB().getRecoSignal().getPM().getID() ]).push_back( topRight);
  (points[ fLOR.getSecondHit().getSignalA().getRecoSignal().getPM().getID() ]).push_back( bottomLeft );
  (points[ fLOR.getSecondHit().getSignalB().getRecoSignal().getPM().getID() ]).push_back( bottomRight );

}

void SDALORCalculateWalkCorrection::prepareTimesForScintCorrection(JPetLOR& fLOR)
{
  double threshold = fThresholds[ fLOR.getFirstHit().getSignalA().getRecoSignal().getPM().getScin().getID() ];
  std::pair<double,double> topLeft= std::make_pair(fLOR.getFirstHit().getSignalA().getRecoSignal().getRecoTimeAtThreshold(threshold), fLOR.getFirstHit().getSignalA().getRecoSignal().getCharge());
  std::pair<double,double> topRight= std::make_pair(fLOR.getFirstHit().getSignalB().getRecoSignal().getRecoTimeAtThreshold(threshold), fLOR.getFirstHit().getSignalB().getRecoSignal().getCharge());
  
  threshold = fThresholds[ fLOR.getSecondHit().getSignalA().getRecoSignal().getPM().getScin().getID() ];
  std::pair<double,double> bottomLeft= std::make_pair(fLOR.getSecondHit().getSignalA().getRecoSignal().getRecoTimeAtThreshold(threshold), fLOR.getSecondHit().getSignalA().getRecoSignal().getCharge());
  std::pair<double,double> bottomRight= std::make_pair(fLOR.getSecondHit().getSignalB().getRecoSignal().getRecoTimeAtThreshold(threshold), fLOR.getSecondHit().getSignalB().getRecoSignal().getCharge());
  
  std::pair<double, double> top = std::make_pair( topLeft.first - topRight.first, 1.0/sqrt(topLeft.second) - 1.0/sqrt(topRight.second) );
  std::pair<double, double> bottom = std::make_pair( bottomLeft.first - bottomRight.first, 1.0/sqrt(bottomLeft.second) - 1.0/sqrt(bottomRight.second) );
  
  (points[ fLOR.getFirstHit().getSignalA().getRecoSignal().getPM().getScin().getID() ]).push_back( top );
  
  (points[ fLOR.getSecondHit().getSignalA().getRecoSignal().getPM().getScin().getID() ]).push_back( bottom );

  fTOFs.push_back( std::make_pair( (topLeft.first+ topRight.first) / 2.0 - (bottomLeft.first + bottomRight.first) / 2.0  , 1.0/sqrt(topLeft.second) + 1.0/sqrt(topRight.second) - 1.0/sqrt(bottomLeft.second) - 1.0/sqrt(bottomRight.second) ) );

}

void SDALORCalculateWalkCorrection::prepareTimesUsingTOT(JPetLOR& fLOR)
{
  double threshold = fThresholds[ fLOR.getFirstHit().getSignalA().getRecoSignal().getPM().getScin().getID() ];
  std::pair<double,double> topLeft= std::make_pair(fLOR.getFirstHit().getSignalA().getRecoSignal().getRecoTimeAtThreshold(threshold), fLOR.getFirstHit().getSignalA().getPhe());
  std::pair<double,double> topRight= std::make_pair(fLOR.getFirstHit().getSignalB().getRecoSignal().getRecoTimeAtThreshold(threshold), fLOR.getFirstHit().getSignalB().getPhe());
  
  threshold = fThresholds[ fLOR.getSecondHit().getSignalA().getRecoSignal().getPM().getScin().getID() ];
  std::pair<double,double> bottomLeft= std::make_pair(fLOR.getSecondHit().getSignalA().getRecoSignal().getRecoTimeAtThreshold(threshold), fLOR.getSecondHit().getSignalA().getPhe());
  std::pair<double,double> bottomRight= std::make_pair(fLOR.getSecondHit().getSignalB().getRecoSignal().getRecoTimeAtThreshold(threshold), fLOR.getSecondHit().getSignalB().getPhe());
  
  std::pair<double, double> top = std::make_pair( topLeft.first - topRight.first, 1.0/sqrt(topLeft.second) - 1.0/sqrt(topRight.second) );
  std::pair<double, double> bottom = std::make_pair( bottomLeft.first - bottomRight.first, 1.0/sqrt(bottomLeft.second) - 1.0/sqrt(bottomRight.second) );
  
  (points[ fLOR.getFirstHit().getSignalA().getRecoSignal().getPM().getScin().getID() ]).push_back( top );
  
  (points[ fLOR.getSecondHit().getSignalA().getRecoSignal().getPM().getScin().getID() ]).push_back( bottom );

  fTOFs.push_back( std::make_pair( (topLeft.first+ topRight.first) / 2.0 - (bottomLeft.first + bottomRight.first) / 2.0  , 1.0/(topLeft.second) + 1.0/(topRight.second) - 1.0/(bottomLeft.second) - 1.0/(bottomRight.second) ) );

}

void SDALORCalculateWalkCorrection::exec()
{
  fReader->getEntry(fEvent);
  //exctract data for each strip seperately
  JPetLOR& fLOR = dynamic_cast< JPetLOR& > ( fReader->getData() );

  prepareTimesForScintCorrection(fLOR);
  //prepareTimesForPMTCorrection(fLOR);
  //prepareTimesUsingTOT(fLOR);
  
  fEvent++;
}
