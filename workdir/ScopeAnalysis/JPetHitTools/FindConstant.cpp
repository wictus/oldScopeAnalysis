#include "./FindConstant.h"

FindConstant::FindConstant(const std::vector<double>& inputEvents, const TString file, const double sourcePos, const int scinID)
{
	scintillatorID = scinID;
	EXPEvents = inputEvents;
	minBin = 0;
	maxBin = 500; //500
	lowerCut = 250;
	upperCut = 380;
	fileName = file;
	sourcePosition = sourcePos;
	gErrorIgnoreLevel = kFatal;
	binNumber = 250;//(maxBin - minBin)/20.0;
	
	
	EXPHisto = new TH1F("Experimental charge histogram", "expHisto", binNumber, minBin, maxBin);
	SIMHisto = new TH1F("Simulated energy histogram", "simHisto", binNumber, minBin, maxBin);
        SIMHisto->SetLineColor(kRed);
        SIMHisto->GetXaxis()->SetTitle("Energy [keV]" );
        SIMHisto->GetYaxis()->SetTitle("Counts" );
        EXPHisto->GetXaxis()->SetTitle("Charge [pC]" );
        EXPHisto->GetYaxis()->SetTitle("Counts" );
	
        bestChi2 = 999999, bestNorm = 1, bestAlpha = 1, bestNumberOfBins = 0, energyResolution = 0;
	

	filePath = fileName;
	filePath = filePath(filePath.Last('/')+1, filePath.Length()-1);
        filePath = filePath(0, filePath.Last('.'));
	std::stringstream buf;
	buf << sourcePosition;
        TString title = "FitResults" + filePath+buf.str() + "/";
        TUnixSystem* system = new TUnixSystem();
        system->mkdir( (std::string(title)).c_str(), 1);
	
	filePath = title;	
}

double FindConstant::findBestBeta(const double start, const double stop, const double step, bool useFit)
{
    std::vector<double> checkedBetaValues;
	std::vector<double> chi2;
	
	for(double energyResolution = start; energyResolution < stop; energyResolution+=step)
	{
	fillEXPHisto(); 	//fills the histogram without prescaling
        produceSIMEvents( SIMEvents, energyResolution );
        fillSIMHisto();

        aproximateParameters();
        fillSIMHisto(initialNorm, initialAlpha);
	
        performFit(energyResolution);
 	fillSIMHisto(bestNorm, bestAlpha);

	double lowerBin = double(SIMHisto->GetBinCenter(lowerCut/bestAlpha));
	double higherBin = double( SIMHisto->GetBinCenter(upperCut/bestAlpha));
	
	bestNumberOfBins= higherBin-lowerBin;      
	INFO( Form("Beta equal to %f was fitted with chi2: %f for number of bins: %f", energyResolution, compareHistogramsByChi2(bestNorm, bestAlpha)/(bestNumberOfBins-3)  , bestNumberOfBins) );
	
	checkedBetaValues.push_back(energyResolution);
	chi2.push_back(compareHistogramsByChi2(bestNorm, bestAlpha));

	fillEXPHisto(1.0/bestNorm, 1.0/bestAlpha);
	fillSIMHisto();
	saveFittedHisto(energyResolution);        
	}

	if( useFit )
	  return drawChi2AndFitPol2(checkedBetaValues, chi2);
	else
	{
	  auto minimumIndex = std::minmax_element(chi2.begin(),chi2.end());
	  auto minimum = minimumIndex.first - chi2.begin() ;
	  double foundMinimum = checkedBetaValues.at(minimum) ;
	  
	  return foundMinimum;
	}
}


double FindConstant::execute()
{       
		 
        energyResolution = findBestBeta(0.5, 2.5, 0.1, true);
	
	energyResolution = findBestBeta( energyResolution - 0.4, energyResolution+0.4 , 0.025, false);
	
 	doFinalFit(energyResolution);
	
	lowErrorEnergyResolution = energyResolution - findLowResolutionError(energyResolution);
	
	INFO( Form( "Found lower error: %f", lowErrorEnergyResolution ) );
	
	highErrorEnergyResolution = findHighResolutionError(energyResolution)- energyResolution;
	
	INFO( Form( "Found higher error: %f", highErrorEnergyResolution ) );
	
	saveFitResultToTxt("fitResults.txt");
	saveErrorsResults("fitResults.txt");
	
        EXPHisto->Reset();
        SIMHisto->Reset();
	delete EXPHisto;
	delete SIMHisto;
        return bestAlpha;
}

void FindConstant::saveErrorsResults( const std::string& name )
{
        std::ofstream myfile;
        myfile.open( name.c_str(), std::ios::app);

        if (myfile.is_open() )
        {
                myfile << energyResolution << "\t"  << lowErrorEnergyResolution << "\t" << highErrorEnergyResolution << std::endl;
        }

        myfile.close();
}

double FindConstant::findHighResolutionError(const double eRes)
{
  double chi2 = 0, bins = 0;
  double resolution = eRes;
  while(true) {
	fillEXPHisto();
        produceSIMEvents( SIMEvents, resolution );
        fillSIMHisto(bestNorm, bestAlpha);

	double lowerBin = double(SIMHisto->GetBinCenter(lowerCut/bestAlpha));
	double higherBin = double( SIMHisto->GetBinCenter(upperCut/bestAlpha));
	bins = higherBin - lowerBin;
	chi2 = compareHistogramsByChi2(bestNorm, bestAlpha);
	
	std::cout << chi2 << "  " << resolution << std::endl;
	
	if( chi2 > bestChi2 + 3.67 )
	  break;
	else 
	  resolution=resolution+0.01;
	
	fillEXPHisto(1.0/bestNorm, 1.0/bestAlpha);
	fillSIMHisto();
	saveFittedHisto(resolution);        	  
  }
  INFO( Form( "Found high error at: %f", resolution ) );
  
  return resolution;
}

double FindConstant::findLowResolutionError(const double eRes)
{
  double chi2 = 0, bins = 0;
  double resolution = eRes;
  while(true) {
	fillEXPHisto();
	
        produceSIMEvents( SIMEvents, resolution );
        fillSIMHisto(bestNorm, bestAlpha);

	double lowerBin = double(SIMHisto->GetBinCenter(lowerCut/bestAlpha));
	double higherBin = double( SIMHisto->GetBinCenter(upperCut/bestAlpha));
	bins = higherBin - lowerBin;
	chi2 = compareHistogramsByChi2(bestNorm, bestAlpha);
	
	std::cout << chi2 << "  " << resolution << std::endl;
	if( chi2 > bestChi2 + 3.67 )
	  break;
	else 
	  resolution=resolution-0.01;
	
	fillEXPHisto(1.0/bestNorm, 1.0/bestAlpha);
	fillSIMHisto();
	saveFittedHisto(resolution);        	
  }
  
  INFO( Form( "Found low error at: %f", resolution ) );
  
  return resolution;
}


void FindConstant::doFinalFit(const double eRes)
{
	fillEXPHisto();
        produceSIMEvents( SIMEvents, eRes );
        fillSIMHisto();

        aproximateParameters();
        fillSIMHisto(initialNorm, initialAlpha);
	
        performFit(eRes);
 	fillSIMHisto(bestNorm, bestAlpha);
			

	
	double lowerBin = double(SIMHisto->GetBinCenter(lowerCut/bestAlpha));
	
	double higherBin = double( SIMHisto->GetBinCenter(upperCut/bestAlpha));
		
	bestNumberOfBins= higherBin-lowerBin;

	bestChi2 = compareHistogramsByChi2(bestNorm, bestAlpha);

	INFO( Form("Beta equal to %f was fitted with chi2: %f for number of bins: %f", energyResolution, bestChi2/ (bestNumberOfBins - 3.0), bestNumberOfBins) );

	fillEXPHisto(1.0/bestNorm, 1.0/bestAlpha);
	fillSIMHisto();
	saveFittedHisto(energyResolution);        
}


void FindConstant::fillEXPHisto(const double normalisation, const double alpha)
{
        EXPHisto->Reset();
        for(unsigned int i = 0; i < EXPEvents.size(); i++)
        {
                EXPHisto->Fill(EXPEvents[i] / alpha, 1.0/normalisation);
        }

}

void FindConstant::produceSIMEvents( std::vector<double>& SIMEvents, const double eRes)
{

    if(SIMEvents.size() != 0 )
      SIMEvents.clear();
        // DEKLARACJE ZMIENNYCH
        double przekroj, E_0, E, stala, fi, firad, pi;
        int i;
        double T, kos, x, y, max, Eprim;
        TRandom3 los;
        double rozdzielczosc, g, sigma, rozmyciex, Troz;

        // INICJALIZACJIA
         przekroj = 0.0;
        E = 1.0;
        stala = pow((2.82*1E-13),2.0) * 0.5;
        E_0 = 511.0      ;
        fi = 0.0;
        i = 0;
        firad=0;
        pi = 3.14159265358979323846;
        kos = 0.0;
        T = 0.0;
        Eprim = 0.0;
        rozdzielczosc =0.0;
        g= 0.0;
        sigma = 0.0;
        rozmyciex =0.0;
        Troz = 0.0;     //Ek. Kin. rozmyta

        // jeden stopien to okolo 0.0175 radiana  bo 1* pi /180 = 0.0175
        rozdzielczosc = eRes;

        fi=0;

        firad = fi * pi/180.0;
        E = E_0 * 511.0 /(511.0 + E_0*( 1- cos(firad)));

        max = E * (2.0*E/511.0)/(1.0+2.0*E/511.0);

        //// PETLA DO RYSOWANIA WIDMA

        for (i=0;i<1E6;) {



        x = los.Rndm()*max;

        y = los.Rndm()*10.0;
        g = los.Gaus(0.0,1.0);
        T = x;
        kos = (511.0/E * T + T - E)/(T-E);
        Eprim = E - T;

        przekroj =  2.0 * 3.14 * stala * pow(Eprim/E,2.0) *(E/Eprim + Eprim/E - 1.0 + pow(kos,2.0) ) * ((511.0/E + 1.0)/(T-E)- (511.0/E * T + T - E)/(pow((T-E),2.0)))*(-1.0)*1E27;



        if(przekroj>y){

        sigma = rozdzielczosc*sqrt(x);
        rozmyciex= sigma * g;
        Troz = x + rozmyciex;
        SIMEvents.push_back(Troz);
        i++;
        }


        }
}

void FindConstant::fillSIMHisto(const double normalisation, const double alpha)
{
        SIMHisto->Reset();
        for(unsigned int i = 0; i < SIMEvents.size(); i++)
        {
                SIMHisto->Fill(SIMEvents[i] / alpha, 1.0/normalisation);
        }

}

void FindConstant::aproximateParameters()
{
        if(NULL == EXPHisto && NULL == SIMHisto)
        {
                ERROR( Form ("Not initialised histograms in findEnergyConstant! Exiting program") );
                std::exit(1);
        }
	initialWidthRatio = estimateWidthRatio();

        fillSIMHisto( 1.0, initialWidthRatio);

        initialHeightRatio =  estimateHeightRatio();

        initialNorm = initialHeightRatio;
        initialAlpha = initialWidthRatio;

        SIMHisto->GetXaxis()->SetRangeUser( minBin, maxBin);
        EXPHisto->GetXaxis()->SetRangeUser( minBin, maxBin);

}

double FindConstant::estimateWidthRatio()
{
	double meanForSimHisto = SIMHisto->GetMean() ;

        SIMHisto->GetXaxis()->SetRangeUser( meanForSimHisto, maxBin);
        EXPHisto->GetXaxis()->SetRangeUser( EXPHisto->GetMean()*1.2, maxBin );

        initialWidthRatio = ( (double)SIMHisto->GetBinCenter( (SIMHisto->GetMaximumBin( )) ) )
                                /  ( (double) EXPHisto->GetBinCenter( EXPHisto->GetMaximumBin() ) );
        SIMHisto->GetXaxis()->SetRangeUser( minBin, maxBin);
        EXPHisto->GetXaxis()->SetRangeUser( minBin, maxBin);
	
	return initialWidthRatio;
}

double FindConstant::estimateHeightRatio()
{
        double meanForSimHisto = SIMHisto->GetMean();
        SIMHisto->GetXaxis()->SetRangeUser( meanForSimHisto, maxBin);
        EXPHisto->GetXaxis()->SetRangeUser( EXPHisto->GetMean()*1.2, maxBin );

        heighestCountsInSIM = (double)SIMHisto->GetBinContent( (SIMHisto->GetMaximumBin( )) );
        heighestCountsInEXP = ( (double)EXPHisto->GetBinContent(EXPHisto->GetMaximumBin() ) );

        SIMHisto->GetXaxis()->SetRangeUser( minBin, maxBin);
        EXPHisto->GetXaxis()->SetRangeUser( minBin, maxBin);

        initialHeightRatio =  heighestCountsInSIM / heighestCountsInEXP;

	return initialHeightRatio;
}

void FindConstant::performFit(const double resolution)
{
    bestAlpha = findBestAlpha(resolution);
    bestNorm = findBestNorm(bestAlpha);
}

double FindConstant::findBestAlpha(const double resolution)
{
  std::vector<double> alphas, chi2s;
  for(double currentAlpha = 0.9*initialAlpha; currentAlpha < 1.1*initialAlpha; currentAlpha+=initialAlpha*0.01)
  {           
		bestNorm = findBestNorm(currentAlpha);
		fillSIMHisto(bestNorm , currentAlpha);
		chi2s.push_back( compareHistogramsByChi2( bestNorm, currentAlpha ) );
		alphas.push_back( currentAlpha );
  }

  auto minChi2 = std::minmax_element( chi2s.begin(), chi2s.end() );

  TCanvas* c1 = new TCanvas();
  TGraph* minimumAlpha = new TGraph(chi2s.size(), &alphas[0], &chi2s[0]);
  
  TString graphTitle = "Chi2 vs #alpha for energy resolution ";
  graphTitle+= resolution;
  
  minimumAlpha->SetTitle(graphTitle);
  minimumAlpha->GetXaxis()->SetTitle("#alpha");
  minimumAlpha->GetYaxis()->SetTitle("Chi2");
  minimumAlpha->SetLineColor(2);
  minimumAlpha->SetLineWidth(4);
  minimumAlpha->SetMarkerColor(4);
  minimumAlpha->SetMarkerStyle(21);
  minimumAlpha->Draw("AP");
  gStyle->SetOptFit(1);
  
  TString title="Chi2Plot_";
  
  std::stringstream name;
  name << "_bestAlpha_" << alphas[ minChi2.first - chi2s.begin() ] << "_ForRes_" << resolution	<<"_ForScint_"<<scintillatorID ; ;
  
  title+=name.str();
  
//   c1->SaveAs( (filePath+title+".png") );
//   c1->SaveAs( (filePath+title+".root") );

  return alphas[ minChi2.first - chi2s.begin() ];
}

double FindConstant::findBestNorm(const double alpha)
{
  std::vector <double> norms, chi2s;
  for(double currentNorm = 0.8*initialNorm; currentNorm < 1.4*initialNorm; currentNorm+=initialNorm*0.01)
                {
                fillSIMHisto( currentNorm, alpha);
                chi2s.push_back( compareHistogramsByChi2( currentNorm, alpha) );
		norms.push_back( currentNorm );
		}
		
  TCanvas* c1 = new TCanvas();
  TGraph* minimumNorm = new TGraph(chi2s.size(), &norms[0], &chi2s[0]);
  
  TString graphTitle =   "Chi2 vs normalisation ";
  graphTitle+= alpha;
  
  minimumNorm->SetTitle( graphTitle );
  minimumNorm->GetXaxis()->SetTitle("Normalisation");
  minimumNorm->GetYaxis()->SetTitle("Chi2");
  minimumNorm->SetLineColor(2);
  minimumNorm->SetLineWidth(4);
  minimumNorm->SetMarkerColor(4);
  minimumNorm->SetMarkerStyle(21);
  minimumNorm->Draw("AP");
  gStyle->SetOptFit(1);


  TString title = "Chi2Plot_";
  
  auto minChi2 = std::minmax_element( chi2s.begin(), chi2s.end() );
  
  std::stringstream name;
  name << "_bestNorm_" <<  norms[ minChi2.first - chi2s.begin() ] << "ForAlpha_" << alpha <<"_ForScint_"<<scintillatorID ;
  
  title+=name.str();
  
//   c1->SaveAs( (filePath+title+".png") );
//   c1->SaveAs( (filePath+title+".root") );
 
  return norms[ minChi2.first - chi2s.begin() ];
}

double FindConstant::compareHistogramsByChi2( const double normalisation, const double alpha)
{
        double chi2 = 0.0;
	
        for(unsigned int i = 0; i < EXPHisto->GetSize() - (unsigned int)2; i++)
        {
	  
                if( 0 == EXPHisto->GetBinContent(i) && 0 == SIMHisto->GetBinContent(i) )
                        continue;

                if( double(SIMHisto->GetBinCenter(i)) > lowerCut/alpha && double( SIMHisto->GetBinCenter(i)) < upperCut/alpha)
                {
                        chi2 +=  pow( ( double( SIMHisto->GetBinContent(i) ) - double( EXPHisto->GetBinContent(i) ) ) , 2.0)
                        / ( double( EXPHisto->GetBinContent(i) ) + ( double( SIMHisto->GetBinContent(i) ) / normalisation ) ) ;
                }
        }
        return chi2;
}

void FindConstant::saveFittedHisto(const double energyResolution)
{
		TCanvas* c1 = new TCanvas();
                SIMHisto->Draw();
                EXPHisto->Draw("same");

                std::stringstream streamForEResExtraction;
                streamForEResExtraction << energyResolution;
                SIMHisto->SetTitle( (streamForEResExtraction.str()).c_str() );
                streamForEResExtraction << "beta_for_scinID_" << scintillatorID;
                TString title= streamForEResExtraction.str();
                title+="position";
                title+=sourcePosition;
                gStyle->SetOptStat(0);
                c1->SaveAs( filePath+title +".png");
                c1->SaveAs( filePath+title +".root");

}

void FindConstant::saveSIMHisto()
{
                TCanvas* c1 = new TCanvas();

                SIMHisto->Draw();

                filePath = fileName;
                filePath = filePath(filePath.Last('/')+1, filePath.Length()-1);
                filePath = filePath(0, filePath.Last('.'));
                TString title = "FitResults" + filePath + "/";
                TUnixSystem* system = new TUnixSystem();
                system->mkdir( (std::string(title)).c_str(), 1);
                std::stringstream streamForEResExtraction;
                streamForEResExtraction << scintillatorID;
                title+= streamForEResExtraction.str();
                title+="position";
                title+=sourcePosition;
                title+="SIMHisto.png";
                gStyle->SetOptStat(0);
                c1->SaveAs( title);

}

void FindConstant::saveEXPHisto()
{
                TCanvas* c1 = new TCanvas();

                EXPHisto->Draw();

                filePath = fileName;
                filePath = filePath(filePath.Last('/')+1, filePath.Length()-1);
                filePath = filePath(0, filePath.Last('.'));
                TString title = "FitResults" + filePath + "/";
                TUnixSystem* system = new TUnixSystem();
                system->mkdir( (std::string(title)).c_str(), 1);
                std::stringstream streamForEResExtraction;
                streamForEResExtraction << scintillatorID;
                title+= streamForEResExtraction.str();
                title+="position";
                title+=sourcePosition;
                title+="EXPHisto.png";
                gStyle->SetOptStat(0);
                c1->SaveAs( title);

}

void FindConstant::saveFitResultToTxt(std::string name)
{
        std::ofstream myfile;
        myfile.open( name.c_str(), std::ios::app);

        if (myfile.is_open() )
        {
//                myfile << "Final fit result for data:" << filePath  << " for scintillator: " << scintillatorID << " for position " << sourcePosition  <<"   alpha: " << bestAlpha << " and beta: " << energyResolution << " and chi2: " << bestChi2 << " for " << bestNumberOfBins << " bins " <<std::endl;
		  myfile << filePath  << "\t" << scintillatorID << "\t" << sourcePosition  << "\t" << bestAlpha << "\t" << energyResolution << "\t" << bestChi2 << "\t" << bestNumberOfBins << "\t";
        }

        myfile.close();

}

std::vector< std::pair<double,double> > FindConstant::extractPointValuesAroundMinimum(const std::vector< std::pair<double,double>>& vec)
{
  std::vector<std::pair<double,double> > exctratedPoints;
  
  auto minChi2 = std::minmax_element( vec.begin(), vec.end() );  
  auto minimum = minChi2.first - vec.begin() ;
  
  double foundMinimum = vec.at(minimum).first ;
    
  for(unsigned int currentPoint = minimum ; currentPoint < (vec.size()-1) && (20*foundMinimum) > vec[currentPoint].first ; currentPoint++)
  {
    exctratedPoints.push_back( vec[currentPoint] );
    if( currentPoint+1 == vec.size()-1 )
    {
	currentPoint+=1;
	exctratedPoints.push_back( vec[currentPoint] );
    }
  }
  
 for(int currentPoint = minimum-1 ;  currentPoint != -1 && (20*foundMinimum) > vec[currentPoint].first ; currentPoint-=1)
  {
    exctratedPoints.push_back( vec[currentPoint] );
    if( currentPoint-1 == 0 )
    {
	currentPoint-=1;
	exctratedPoints.push_back( vec[currentPoint] );
    }
  }
  
  return exctratedPoints;
}


double FindConstant::drawChi2AndFitPol2(const std::vector<double>& res, const std::vector<double>& chi2)
{
        TCanvas* c1 = new TCanvas();
        std::stringstream streamForEResExtraction;
        streamForEResExtraction << scintillatorID << "_beta_"<< energyResolution;
	
	std::vector< std::pair< double, double >> points;
	
	for(unsigned int i = 0; i < res.size(); i++)
	{
	  points.push_back(std::make_pair( chi2[i], res[i]) );
	}
	
	std::vector< std::pair< double, double > > pointsAroundMinimum = extractPointValuesAroundMinimum(points);
	
		
        std::vector<double> Chi2ToFit, ResToFit;
        for(size_t i = 0; i < pointsAroundMinimum.size(); i++)
        {
                Chi2ToFit.push_back( pointsAroundMinimum[i].first );
                ResToFit.push_back( pointsAroundMinimum[i].second);
        }

        TMultiGraph* m = new TMultiGraph();

        TGraph* gr = new TGraph(Chi2ToFit.size(), &ResToFit[0], &Chi2ToFit[0]);
        gr->SetTitle("Chi2 vs #beta");
        gr->GetXaxis()->SetTitle("#beta (#sqrt{keV})");
        gr->GetYaxis()->SetTitle("Chi2 for best fit");
        gr->SetLineColor(2);
        gr->SetLineWidth(4);
        gr->SetMarkerColor(4);
        gr->SetMarkerStyle(21);
	gr->Draw("AP");
        gStyle->SetOptFit(1);
	
  auto min = std::minmax_element( ResToFit.begin(), ResToFit.end() );  
  auto minimum = ResToFit[min.first - ResToFit.begin()] ;
  auto maximum = ResToFit[min.second - ResToFit.begin()];
        quadraticFit = new TF1("quadraticFit", "[0]* (x - [1])**2 + [2]", minimum, maximum );
        quadraticFit->SetParName(0, "a");
        quadraticFit->SetParName(1, "x_min");
        quadraticFit->SetParName(2, "y_min");
	quadraticFit->SetParameter(0, 1);
        quadraticFit->SetParameter(1, energyResolution);
	quadraticFit->SetParameter(2, bestChi2);
        gr->Fit(quadraticFit,"R");
        quadraticFit->Draw("same");

        c1->SaveAs(  filePath+"/Chi2Plot_strip_fitRegion_"+streamForEResExtraction.str() + expHistoTitle+".png" );
        c1->SaveAs(  filePath+"/Chi2Plot_strip_fitRegion_"+streamForEResExtraction.str() + expHistoTitle+".root" );

	m->Add(gr);

        TGraph* grFull = new TGraph(chi2.size(), &res[0], &chi2[0]);
        grFull->SetTitle("Chi2 vs #beta");
        grFull->GetXaxis()->SetTitle("#beta (#sqrt{keV})");
        grFull->GetYaxis()->SetTitle("Chi2 for best fit");
        grFull->SetLineColor(2);
        grFull->SetLineWidth(4);
        grFull->SetMarkerColor(4);
        grFull->SetMarkerStyle(21);

	m->Add(grFull);

	m->Draw("AP");

        c1->SaveAs( (filePath+"/Chi2Plot_strip_"+streamForEResExtraction.str() + expHistoTitle+".png") );
        c1->SaveAs( (filePath+"/Chi2Plot_strip_"+streamForEResExtraction.str() + expHistoTitle+".root") );

        delete gr;
        delete c1;
	
	return quadraticFit->GetParameter(1);
}

double FindConstant::returnEnergyResolution()
{
	return energyResolution;
}
