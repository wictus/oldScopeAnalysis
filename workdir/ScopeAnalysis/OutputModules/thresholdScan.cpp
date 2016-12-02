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
	double energyT = hitFirst.getEnergy();
	double energyB = hitSecond.getEnergy();

    if(energyCuts.size() == 2)
    {
	if( energyT > energyCuts[0] && energyT < energyCuts[1] )
	{
		energyTop.push_back(energyT);
	}

	if( energyB > energyCuts[0] && energyB < energyCuts[1] )
	{
		energyBottom.push_back(energyB);
	}
    }
    else if(energyCuts.size() == 1)
    {
        if( energyT > energyCuts[0] )
        {
                energyTop.push_back(energyT);
        }

        if( energyB > energyCuts[0] )
        {
                energyBottom.push_back(energyB);
        }
    }

  // Save times from signals to file
  for( unsigned int i = 0; i < fThresholds.size(); i++)
  {
    if(energyCuts.size() == 2)
    {
	if( energyT > energyCuts[0] && energyT < energyCuts[1] )
	{
		fDeltaTimesForThresholdsTop[i].push_back( signalFirst.getRecoTimeAtThreshold( (double)fThresholds[i] ) - signalSecond.getRecoTimeAtThreshold( (double)fThresholds[i] ) ); 
	}
	
	if(energyB > energyCuts[0] && energyB < energyCuts[1] )	
	{
		fDeltaTimesForThresholdsBottom[i].push_back( signalThird.getRecoTimeAtThreshold( (double)fThresholds[i] ) - signalFourth.getRecoTimeAtThreshold( (double)fThresholds[i] ) ); 
	}
    }
    else if(energyCuts.size() == 1)
    {
      if(energyT > energyCuts[0] ){
                fDeltaTimesForThresholdsTop[i].push_back( signalFirst.getRecoTimeAtThreshold( (double)fThresholds[i] ) - signalSecond.getRecoTimeAtThreshold( (double)fThresholds[i] ) ); 
	}

      if(energyB > energyCuts[0] )
	{
                fDeltaTimesForThresholdsBottom[i].push_back( signalThird.getRecoTimeAtThreshold( (double)fThresholds[i] ) - signalFourth.getRecoTimeAtThreshold( (double)fThresholds[i] ) ); 
	}
    }
    else{
	ERROR( Form("Bad energy cuts vector provided") );
	}
 }
  fEvent++;
}

void thresholdScan::end()
{

	plotHistosAndGraphs( fDeltaTimesForThresholdsTop, "_TOP_");
	plotHistosAndGraphs( fDeltaTimesForThresholdsBottom, "_BOTTOM_");
	plotEnergyCuts(energyTop, "_TOP_");
	plotEnergyCuts(energyBottom, "_BOTTOM_");
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

          TH1F* energy = new TH1F( title.c_str(), title.c_str() , 250, 0, 500);

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
  std::vector<double> resolutions;
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

  for(unsigned int j = 0; j < fThresholds.size(); j++){
          double thr = fThresholds[j];
          std::stringstream buf; buf << thr;
          std::string title = buf.str();
          title+="mV";
	  title+=name;


          TH1F* deltaT = new TH1F( title.c_str(), title.c_str() , 200, -5000, 5000);

          for(unsigned int i = 0; i < data[j].size(); i++){
		if( 0 == data[j].size())
		  break;
                if( data[j][i] != 0 )
                        deltaT->Fill( data[j][i] );
          }
	
        TCanvas* c1 = new TCanvas();
	  deltaT->Sumw2();
	double eventsNotInRange = deltaT->GetBinContent(0) + deltaT->GetBinContent(201);
	if( eventsNotInRange != deltaT->GetEntries() )
	{
		std::cout<< deltaT->GetEntries() << std::endl;
	        deltaT->Fit("gaus","QI");
       		resolutions.push_back( (double)deltaT->GetFunction("gaus")->GetParameter(2) );
		chi2.push_back( ( (double)deltaT->GetFunction("gaus")->GetChisquare() ) / ( (double)deltaT->GetFunction("gaus")->GetNDF() ) );
	}
	else
	{
		resolutions.push_back(0);
		chi2.push_back(0);
	}
        deltaT->Draw();
	title+=buf.str();
        c1->SaveAs( ((std::string)path+title+".png").c_str() );

        delete deltaT;
  }

  std::ofstream outFile;

  outFile.open( ((std::string)path+name + "resolutions"+buf.str()+".txt").c_str() );

  for(unsigned int i = 0; i < resolutions.size(); i++){
        outFile << fThresholds[i] << "\t"<< resolutions[i] << "\t" << chi2[i] <<  std::endl;
  }

  outFile.close();

  TCanvas* c1 = new TCanvas();
  TGraph* resGraph = new TGraph(resolutions.size(),&fThresholds[0], &resolutions[0]);
  resGraph->SetTitle( "" );
  resGraph->SetMarkerStyle(21);
  resGraph->SetMarkerSize(1.4);
  resGraph->GetXaxis()->SetTitle("Threshold [mV]");
  resGraph->GetYaxis()->SetTitle("Time difference resolution [ps]");
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

 
  resGraph->Draw("AP");
  c1->SaveAs( ((std::string)path+name+buf.str()+"graph.root").c_str() );
  resGraph->Draw("AP");
  c1->SaveAs( ((std::string)path+name+"graph.png").c_str() );

  delete resGraph;

}

