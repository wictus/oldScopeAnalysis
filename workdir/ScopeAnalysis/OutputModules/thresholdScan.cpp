#include "./thresholdScan.h"

ClassImp(thresholdScan);

//standard constructor from AnalysisModule
thresholdScan::thresholdScan(const char* name, const char* title,
                   const char* in_file_suffix, const char* out_file_suffix, const std::vector<double>& thresholds, const std::vector<double>& eCut)
                   : JPetCommonAnalysisModule(name, title, in_file_suffix, out_file_suffix)
{
  setVersion(MODULE_VERSION);
  fThresholds = thresholds;
  energyCuts = eCut;
}

//no specific destructor needed
thresholdScan::~thresholdScan()
{
}

void thresholdScan::begin()
{
  
	for( unsigned int i = 0; i < fThresholds.size(); i++ ){
		std::vector<double> k;
		fDeltaTimesForThresholdsTop.push_back( k );
		fDeltaTimesForThresholdsBottom.push_back( k );
	}
}

void thresholdScan::exec()
{
  
  // Take next entry 
  fReader->getEntry(fEvent);

  // Cast data from the entry into JPetLOR
  const JPetLOR& lor = (JPetLOR&) fReader->getData();
  
  // Extract signals
  const JPetRecoSignal& signalFirst = lor.getFirstHit().getSignalA().getRecoSignal();
  const JPetRecoSignal& signalSecond = lor.getFirstHit().getSignalB().getRecoSignal();
  const JPetRecoSignal& signalThird = lor.getSecondHit().getSignalA().getRecoSignal();
  const JPetRecoSignal& signalFourth = lor.getSecondHit().getSignalB().getRecoSignal();

  const JPetHit& hitFirst = lor.getFirstHit();
  const JPetHit& hitSecond = lor.getSecondHit();
  double pheOne = lor.getSecondHit().getSignalA().getPhe();
  double pheTwo = lor.getSecondHit().getSignalB().getPhe();
  
	double energyT = hitFirst.getEnergy();
	double energyB = hitSecond.getEnergy();
	double chargeOne = signalFirst.getCharge();
	double chargeTwo = signalSecond.getCharge();
	double ampOne = signalFirst.getAmplitude();
	double ampTwo = signalSecond.getAmplitude();
	
	double thresholdForReference = 30;

  // Save times from signals to file
  for( unsigned int i = 0; i < fThresholds.size(); i++)
  {
//     if(energyCuts.size() == 2)
//     {
// 	if( chargeOne > 352 && chargeTwo > 395 )
//  	if( pheOne> 80 && pheOne < 120 && pheTwo > 80 && pheTwo < 120 )
// 	  fDeltaTimesForThresholdsTop[i].push_back( (signalThird.getRecoTimeAtThreshold( (double)fThresholds[i] ) + signalFourth.getRecoTimeAtThreshold( (double)fThresholds[i] ))/2.0  ); 
      
	fDeltaTimesForThresholdsTop[i].push_back( signalFirst.getRecoTimeAtThreshold( (double)fThresholds[i] ) - signalSecond.getRecoTimeAtThreshold( thresholdForReference ) ); 
//	fDeltaTimesForThresholdsTop[i].push_back( signalFirst.getRecoTimeAtThreshold( (double)fThresholds[i] ) - signalSecond.getRecoTimeAtThreshold( (double)fThresholds[i] ) ); 
//  	}

	fDeltaTimesForThresholdsBottom[i].push_back( (signalFirst.getRecoTimeAtThreshold( (double)fThresholds[i] ) + signalSecond.getRecoTimeAtThreshold( (double)fThresholds[i] ) )/2.0  ); 

 }
  fEvent++;
}

void thresholdScan::end()
{

	plotHistosAndGraphs( fDeltaTimesForThresholdsTop, "_timeDiff_");
// 	plotHistosAndGraphs( fDeltaTimesForThresholdsBottom, "_hitTime_");
// 	plotEnergyCuts(energyTop, "_TOP_");
// 	plotEnergyCuts(energyBottom, "_BOTTOM_");
}

void thresholdScan::plotEnergyCuts( std::vector<double>& data, std::string name)
{
  TString filePath = fHeader->getBaseFileName();
  filePath = filePath(filePath.Last('/')+1, filePath.Length()-1);
  filePath = filePath(0, filePath.Last('.'));

  std::stringstream buf;
  buf << fHeader->getSourcePosition();
  buf << "_";
  if( energyCuts.size() == 2 )
        buf << energyCuts[0] <<"-"<<energyCuts[1];
  else if (energyCuts.size() == 1 )
        buf << energyCuts[0] << "-inf";
  gStyle->SetOptFit(1);

  TString path = "ThresholdScan" + filePath+buf.str() + "/";
  TUnixSystem* system = new TUnixSystem();
  system->mkdir( (std::string(path)).c_str(), 1);

          std::string title = buf.str();
          title+=name;

          TH1F* energy = new TH1F( title.c_str(), title.c_str() , 500, 0,1000);

          for(unsigned int i = 0; i < data.size(); i++){
                       energy->Fill( data[i] );
          }
        TCanvas* c1 = new TCanvas();
        energy->Draw();
        c1->SaveAs( ((std::string)path+title+".png").c_str() );

        delete energy;
}

void thresholdScan::plotHistosAndGraphs( std::vector<std::vector<double> >& data, std::string name)
{
  std::vector<double> resolutions, resErrors;
  std::vector<double> chi2;

  TString filePath = fHeader->getBaseFileName();
  filePath = filePath(filePath.Last('/')+1, filePath.Length()-1);
  filePath = filePath(0, filePath.Last('.'));

  std::stringstream buf;
  buf << fHeader->getSourcePosition();
  	if( energyCuts.size() == 2 )
		buf <<"_"<< energyCuts[0] <<"-"<<energyCuts[1];
	else if (energyCuts.size() == 1 )
		buf << "_" <<energyCuts[0] << "-inf";
  gStyle->SetOptFit(1);

  TString path = "ThresholdScan" + filePath+buf.str() + "/";
  TUnixSystem* system = new TUnixSystem();
  system->mkdir( (std::string(path)).c_str(), 1);

  double mean = 0;
  
  for(unsigned int j = 0; j < fThresholds.size(); j++){
          double thr = fThresholds[j];
          std::stringstream buf; buf << thr;
          std::string title = buf.str();
          title+="mV";
	  title+=name;


          TH1F* deltaT = new TH1F( title.c_str(), title.c_str() , 10000, JPetRecoSignalTools::min(data[j]), JPetRecoSignalTools::max(data[j]));

          for(unsigned int i = 0; i < data[j].size(); i++){
		if( 0 == data[j].size())
		  break;
                if( data[j][i] != 0 )
                        deltaT->Fill( data[j][i] );
		
		
          }
          
	mean = deltaT->GetMean();
	delete deltaT;
  }
  
  for(unsigned int j = 0; j < fThresholds.size(); j++){
          double thr = fThresholds[j];
          std::stringstream buf; buf << thr;
          std::string title = buf.str();
          title+="mV";
	  title+=name;


          TH1F* deltaTScaled = new TH1F( title.c_str(), title.c_str() , 2000, 20000, 40000);

          for(unsigned int i = 0; i < data[j].size(); i++){
		if( 0 == data[j].size())
		  break;
                if( data[j][i] != 0 )
                        deltaTScaled->Fill( data[j][i] );
		
		
          }
  
	
        TCanvas* c1 = new TCanvas();
// 	deltaT->Sumw2();
	double eventsNotInRange = deltaTScaled->GetBinContent(0) + deltaTScaled->GetBinContent(201);
	if( eventsNotInRange != deltaTScaled->GetEntries() )
	{
		
		double center = deltaTScaled->GetBinCenter(deltaTScaled->GetMaximumBin());
		TF1* onlyGaus = new TF1("onlyGaus", "gaus",  center*0.8, center*1.2);
		deltaTScaled->Fit("onlyGaus","QIR");
	
		TF1* gausAndPoly = new TF1("gausAndPoly", "gaus(0)+pol2(3)", center*0.8, center*1.2);
		gausAndPoly->SetParameter(0, onlyGaus->GetParameter(0) );
		gausAndPoly->SetParameter(1, onlyGaus->GetParameter(1));
		gausAndPoly->SetParameter(2, onlyGaus->GetParameter(2));
// 		deltaTScaled->Fit("gausAndPoly","QIR");

//        		resolutions.push_back( (double)deltaTScaled->GetFunction("gausAndPoly")->GetParameter(2) );
// 		chi2.push_back( ( (double)deltaTScaled->GetFunction("gausAndPoly")->GetChisquare() ) / ( (double)deltaTScaled->GetFunction("gausAndPoly")->GetNDF() ) );
		resolutions.push_back( (double)deltaTScaled->GetFunction("onlyGaus")->GetParameter(2) );
		resErrors.push_back((double)deltaTScaled->GetFunction("onlyGaus")->GetParError(2));
		
		chi2.push_back( ( (double)deltaTScaled->GetFunction("onlyGaus")->GetChisquare() ) / ( (double)deltaTScaled->GetFunction("onlyGaus")->GetNDF() ) );
	}
	else
	{
		resolutions.push_back(0);
		chi2.push_back(0);
	}
        deltaTScaled->Draw();
	title+=buf.str();
        c1->SaveAs( ((std::string)path+title+".png").c_str() );
        c1->SaveAs( ((std::string)path+title+".root").c_str() );
	
        delete deltaTScaled;
  }

  std::ofstream outFile;

  outFile.open( ((std::string)path+name + "resolutions"+buf.str()+".txt").c_str() );

  for(unsigned int i = 0; i < resolutions.size(); i++){
        outFile << fThresholds[i] << "\t"<< resolutions[i] << "\t" << resErrors[i] <<  std::endl;
  }

  outFile.close();

  TCanvas* c1 = new TCanvas();
  TGraph* resGraph = new TGraph(resolutions.size(),&fThresholds[0], &resolutions[0]);
  resGraph->SetTitle( "" );
  resGraph->SetMarkerStyle(21);
  resGraph->SetMarkerSize(1.4);
  resGraph->GetXaxis()->SetTitle("Threshold [mV]");
  if( !name.compare("_TOP_") )
    resGraph->GetYaxis()->SetTitle("Time difference resolution [ps]");
  else if( !name.compare("_BOTTOM_") )
    resGraph->GetYaxis()->SetTitle("Hit time resolution [ps]");
  resGraph->GetXaxis()->SetLabelFont(42);
  resGraph->GetYaxis()->SetLabelFont(42);
  resGraph->GetXaxis()->SetLabelSize(0.06);
  resGraph->GetYaxis()->SetLabelSize(0.06);
  resGraph->GetYaxis()->SetTitleOffset(1.2);
  resGraph->GetXaxis()->SetTitleFont(42);
  resGraph->GetYaxis()->SetTitleFont(42);
  resGraph->GetXaxis()->SetTitleSize(0.06);
  resGraph->GetYaxis()->SetTitleSize(0.06);

  c1->SetLeftMargin(0.1716621);
  c1->SetRightMargin(0.02815622);
  c1->SetTopMargin(0.02572016);
  c1->SetBottomMargin(0.1738683);

  resGraph->GetYaxis()->SetRangeUser(0, 600);
  resGraph->Draw("AP");
  c1->SaveAs( ((std::string)path+name+buf.str()+"graph.root").c_str() );
  resGraph->Draw("AP");
  c1->SaveAs( ((std::string)path+name+"graph.png").c_str() );

  delete resGraph;

}

