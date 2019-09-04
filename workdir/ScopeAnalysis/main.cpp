//THIS IS A MAIN FILE FOR SOURCE ANALYSIS
#include "../../framework/JPetManager/JPetManager.h"
#include <utility>
#include <map>
#include "TF1.h"
#include "SignalAnalysis/SDARecoOffsetsCalc.h"
#include "SignalAnalysis/SDARecoTimeAtThr.h"
#include "SignalAnalysis/SDAMakePhysSignals.h"
#include "HitAnalysis/SDAMatchHits.h"
#include "HitAnalysis/SDAHitPositions.h"
#include "HitAnalysis/SDAHitFindEnergyConstant.h"
#include "HitAnalysis/SDAHitCalculateEnergy.h"
#include <TString.h>
#include "SignalAnalysis/SDARecoChargeCalc.h"
#include "SignalAnalysis/drawOffset.h"
#include "SignalAnalysis/drawAllOffsets.h"
#include "SignalAnalysis/drawAllCharges.h"
#include "SignalAnalysis/drawAllAmps.h"
#include "LORAnalysis/SDALORCalculateEnergy.h"
#include "LORAnalysis/SDALORCutOnEnergy.h"
#include "LORAnalysis/SDAMatchLORs.h"
#include "LORAnalysis/SDALORCalculateTimeAtThr.h"
#include "LORAnalysis/SDATOFs.h"
#include "LORAnalysis/SDAEstimateTimeCalib.h"
#include "LORAnalysis/SDAEstimateTOFCalib.h"
#include "LORAnalysis/SDADoTimeCalib.h"
#include "LORAnalysis/SDADoTOFCalib.h"
#include "OutputModules/bossOutput.h"
#include "OutputModules/bpOutput.h"
#include "LORAnalysis/SDACalculatePositions.h"
#include "LORAnalysis/SDACalculateTimes.h"
#include "OutputModules/twoDRecoOutput.h"
#include "OutputModules/thresholdScan.h"
#include "LORAnalysis/SDALORCalculateWalkCorrection.h"
#include "LORAnalysis/SDALORApplyWalkCorrectionToTOF.h"
#include "LORAnalysis/SDALORCalculateTOT.h"
#include "OutputModules/TOTVSQ.h"
#include "LORAnalysis/SDALORApplyWalkCorrectionToDeltaT.h"
#include "LORAnalysis/SDALORCalculateEnergyFromCalibration.h"
#include "SignalAnalysis/SDARecoAmplitudeCalc.h"
#include "LORAnalysis/SDALORFindAmplitudeConstant.h"
#include "OutputModules/SDALORExctractAmplitudes.h"
#include "LORAnalysis/SDALORCutOnTOT.h"
#include "SignalAnalysis/SDARecoRisingTimeCalc.h"
#include "SignalAnalysis/SDARecoFallingTimeCalc.h"
#include "SignalAnalysis/SDARecoEnergyResolutionFit.h"
#include "SignalAnalysis/SDARecoAmplitudeCut.h"
#include "SignalAnalysis/SDARecoCutAroundPhotopeak.h"
#include "OutputModules/makeMeanSignal.h"

using namespace std;

    struct PMT 
        {   
	  int id; 
          double p0; 
          double p1; 
        }; 

void testsOfSiPMs(JPetManager& manager, std::vector<double>& thresholds, std::pair<double, double> amplitudeCut);	
void signalAnalysis( JPetManager& manager, const std::vector<double> thresholds, std::pair<double, double> amplitudeCut );
void hitsAnalysis(JPetManager& manager);
void LORAnalysis(JPetManager& manager);
void thresholdAndTimeScan(JPetManager& manager, std::vector<double> thresholds);

int main(int argc, char* argv[])
{
  JPetManager& manager = JPetManager::GetManager();
  manager.ParseCmdLine(argc, argv);
  std::vector<double> thr;
//     thr.push_back(40);
  for(double t = 5.0; t < 50; t+=2.5)
    thr.push_back(t);
  for(double t = 50; t < 110; t+=10)
    thr.push_back(t);
//   thr.push_back(20);
  std::pair<double,double> ampCut;
  ampCut = std::make_pair<>(15,30); // mV
testsOfSiPMs(manager, thr, ampCut);

  // run the analysis
  manager.Run();

}

void testsOfSiPMs(JPetManager& manager, std::vector<double>& thresholds, std::pair<double, double> amplitudeCut)
{
     signalAnalysis(manager,thresholds, amplitudeCut);
//     hitsAnalysis(manager);
//     LORAnalysis(manager); 
//    thresholdAndTimeScan(manager, thresholds);
}

	
void signalAnalysis( JPetManager& manager, const std::vector<double> thresholds, std::pair<double, double> amplitudeCut ){
   // SDAcalib for offsets
   SDARecoOffsetsCalc* offsetsCalib = new SDARecoOffsetsCalc("Module SDARecoOffsetsCalc: calculating offsets of SDA signals", "Write SDA signals with offsets taken as a mean of noise points", 
							     "reco.sig.root", "offsets.sig.root");

   SDARecoChargeCalc* chargeCalc = new SDARecoChargeCalc("Module SDARecoChargeCalc: calculating area of SDA signals", "Write SDA signals with area calculated from first significant point, taking into account offset",
							 "offsets.sig.root", "charge.sig.root");
   
	drawOffset* drawOffsetFor49 = new drawOffset("Module drawOffset: draw offset spectrum for selected PMT", "Saves PNG file of the draw spectrum",
						     "charge.sig.root", "offsets.root",49);
	drawOffset* drawOffsetFor0= new drawOffset("Module drawOffset: draw offset spectrum for selected PMT", "Saves PNG file of the draw spectrum",
						   "charge.sig.root", "offsets.root",0);
	drawOffset* drawOffsetFor44 = new drawOffset("Module drawOffset: draw offset spectrum for selected PMT", "Saves PNG file of the draw spectrum",
						     "charge.sig.root", "offsets.root",44);
	drawOffset* drawOffsetFor45 = new drawOffset("Module drawOffset: draw offset spectrum for selected PMT", "Saves PNG file of the draw spectrum",
						     "charge.sig.root", "offsets.root",45);
// 	manager.AddTask(drawOffsetFor49);
// 	manager.AddTask(drawOffsetFor0);
// 	manager.AddTask(drawOffsetFor44);
// 	manager.AddTask(drawOffsetFor45);

  drawAllOffsets* drawOffsets = new drawAllOffsets("Module drawAllOffsets: draw offsets for all PMT", "Finds heighest histo and plots it first, then the rest",
						   "charge.sig.root", "offsets.root");
  drawAllCharges* drawCharges = new drawAllCharges("Module drawAllCharges: draw charges for all PMT", "Finds heighest histo and plots it first, then the rest",
						   "charge.sig.root", "charges.root");
  drawAllAmps* drawAmps = new drawAllAmps("Module drawAllamps: draw amps for all PMT", "Finds heighest histo and plots it first, then the rest",
					  "amps.sig.root", "kiko.root");

	SDARecoRisingTimeCalc* drawRise = new SDARecoRisingTimeCalc("Module SDARecoRisingTimeCalc: draw rise times for all PMT", "same as drawAllAmpls but plots rise times",
								    "amps.sig.root", "kiko.root");
	
	SDARecoFallingTimeCalc* drawFall = new SDARecoFallingTimeCalc("Module SDARecoFallngTimeCalc: draw fall times for all PMT", "same as drawAllAmpls but plots fall times",
								      "charge.sig.root", "kiko.root");

	SDARecoAmplitudeCalc* amplitudeCalc = new SDARecoAmplitudeCalc("Module SDARecoAmplitudeCalc: calculates signal amplitudes","",
								       "photoCut.sig.root","amps.sig.root");
	
	
	TF1 PMTcurve("gain","[0]*x**[1]",0,10);
	PMTcurve.SetParameter(0,20.6E-4);
	PMTcurve.SetParameter(1,6.89);
	
	std::map<int,TF1> curves;
	curves[1] = PMTcurve;
	curves[2] = PMTcurve;
	curves[3] = PMTcurve;
	curves[4] = PMTcurve;
	
	std::map<int, double> voltages;
	voltages[1]=2.250;
	voltages[2]=2.250;
	voltages[3]=2.250;
	voltages[4]=2.250;
	
	std::map< std::string , std::pair< double, double > > peaks;/*
  	peaks[ "1400" ] = std::make_pair<double, double>( 74.6584, 1.91725 );
	peaks[ "1500" ] = std::make_pair<double, double>( 104.233, 2.60083 );
	peaks[ "1550" ] = std::make_pair<double, double>( 121.402, 2.9758 );
	peaks[ "1300" ] = std::make_pair<double, double>( 51.6562, 1.96618 );
	*/
	peaks[ "1300" ] = std::make_pair<double, double>( 57.6977, 0.977808 );
	peaks[ "1400" ] = std::make_pair<double, double>( 83.9948, 1.41527 );
	peaks[ "1500" ] = std::make_pair<double, double>( 115.764 , 1.95342 );
	peaks[ "1600" ] = std::make_pair<double, double>( 154.788, 2.53949 );
	peaks[ "1700" ] = std::make_pair<double, double>( 201.339, 3.32995 );
// 	peaks[ "1550" ] = std::make_pair<double, double>( 133.749, 2.14108 );
	peaks[ "1550" ] = std::make_pair<double, double>( 129.862, 2.08696 ); //Plastik
	peaks[ "1750" ] = std::make_pair<double, double>( 226.507, 3.64842 );
	peaks[ "2850" ] = std::make_pair<double, double>( 2.08894e+02, 2.26280e+01 );
	peaks[ "2618" ] = std::make_pair<double, double>(1.06391e+02 , 1.12350e+01 );
	
	
 	SDARecoCutAroundPhotopeak* cutOnPhotopeak = new SDARecoCutAroundPhotopeak("Module SDARecoCutAroundPhotopeak: cuts around photopeak region", "only signals one sigma away from the photopeak survive", "charge.sig.root", "photoCut.sig.root", peaks);
	
	makeMeanSignal* sabatMeanSignal = new makeMeanSignal(" "," ", "photoCut.sig.root","temp.root", 4);
	
	SDARecoEnergyResolutionFit* findResolution = new SDARecoEnergyResolutionFit("Module SDARecoEnergyResolutionFit: fit gaussian to the photopeak", "Produces file with position and gaussian fit result",
										    "charge.sig.root", "energyFit.sig.root" );
	
	SDARecoAmplitudeCut* amplitudeCuts = new SDARecoAmplitudeCut("Module SDARecoAmplitudeCut: cuts on amplitude", "Only signals in amplitude range are saved",
								     "amps.sig.root", "ampCut.sig.root", amplitudeCut);

	SDARecoTimeAtThr* timeAtThr = new SDARecoTimeAtThr("Module SDARecoTimeAtThr: calculates time at given threshold","Interpolates time between points as a line and check its crossing with threshold levels",
					 "amps.sig.root","times.sig.root", thresholds);

	
	SDAMakePhysSignals* convertToPhys = new SDAMakePhysSignals("Module SDAMakePhysSignals: convert RecoSignals to Phys signals", "Produces phys signals with recosignal setted as their parents",
								   "times.sig.root", "afterPhysConvert.sig.root", curves, voltages);  
	

	
//      	manager.AddTask(offsetsCalib);
//      	manager.AddTask(chargeCalc);
// 	manager.AddTask(findResolution);	
// 	manager.AddTask(cutOnPhotopeak);
// 	manager.AddTask(sabatMeanSignal);
// 	manager.AddTask(drawOffsets);
 	manager.AddTask(drawCharges);
//    	manager.AddTask(amplitudeCalc);
//  	manager.AddTask(amplitudeCuts);
//    	manager.AddTask(drawAmps);
//  	manager.AddTask(drawRise);
//   	manager.AddTask(drawFall);
//    	manager.AddTask(timeAtThr);
//    	manager.AddTask(convertToPhys);
}

void hitsAnalysis(JPetManager& manager)
{
    SDAMatchHits* matchHits = new SDAMatchHits("Module SDAMatchHits: save as hits those events which have exactly two signals in one TSlot", "Produces root file with JPetHits from root file with JPetPhysSignals ","afterPhysConvert.sig.root", "matchedHits.hits.root");

    std::vector< std::pair<int,double>> dECalibration;
    std::pair<int,double> topStripE(72, 6.468);  
    std::pair<int,double> bottomStripE(71, 6.6);
    dECalibration.push_back(topStripE);
    dECalibration.push_back(bottomStripE);

    SDAHitFindEnergyConstant* makeFit = new SDAHitFindEnergyConstant("Module SDAHitFindEnergyConstant: finds energy scaling factor", "Reads charges from both PMTs forming a Hit, takes avereage and fits to the charge spectrum MC simulation", "matchedHits.hits.root", "foundEnergy.hits.root");

    SDAHitCalculateEnergy* calculateEnergy = new SDAHitCalculateEnergy("Module SDAHitCalculateEnergy: calculates energy of JPetHits", "Reads arithmetic sum of charges from JPetPhys and scales them with factor from SDAHitFindEnergyConstant module", "foundEnergy.hits.root", "calculatedEnergy.hits.root");
	
     manager.AddTask(matchHits);
    
//    manager.AddTask(makeFit);
    //manager.AddTask(calculateEnergy);
    
}

void LORAnalysis(JPetManager& manager)
{
  SDAMatchLORs* matchLORs = new SDAMatchLORs("Module SDAMatchLOR: save as LOR those hits which are in same TSlot and come from different scintillators", "Produces root file with JPetLORs from root file with JPetHits, JPetLORs are empty ", "matchedHits.hits.root", "matchedLORs.lors.root");
  
    std::vector< std::pair<int,double>> dECalibration;
    std::pair<int,double> topStripE(72, 6.455);  
    std::pair<int,double> bottomStripE(71, 6.574);
//    std::pair<int,double> topStripE(86,7.603 );  
//    std::pair<int,double> bottomStripE(87,7.426 );
    dECalibration.push_back(topStripE);
    dECalibration.push_back(bottomStripE);
 
  SDALORCalculateEnergy* calculateEnergy = new SDALORCalculateEnergy("Module SDAHitCalculateEnergy: calculates energy of JPetHits", "Reads arithmetic sum of charges from JPetPhys and scales them with factor from SDAHitFindEnergyConstant module", "matchedLORs.lors.root", "calculatedEnergy.lors.root", dECalibration);
  manager.AddTask(matchLORs);
//  manager.AddTask(calculateEnergy);
}

void thresholdAndTimeScan(JPetManager& manager, std::vector<double> thresholds)
{
      std::vector<std::vector<double> >cuts;
    std::vector<double> cut;
//     for(int i = 0; i < 8; i++)
//     {   
//         cut.push_back(50+i*50);
//         cut.push_back(100+i*50);
//         cuts.push_back(cut);
//         cut.clear();
//     }   
        cut.push_back(0);
        cuts.push_back(cut);
	cut.clear();
		
    SDALORCalculateTimeAtThr* calculateTimesAtThrs = new SDALORCalculateTimeAtThr("Module: SDALORCalculateTimeAtThr is calculating times at threshold before scan!","Reads file with LORs and calculates times at thr for signals", "matchedLORs.lors.root", "timeAtThrFilled.lors.root", thresholds);
//    //manager.AddTask(calculateTimesAtThrs);
    for(int unsigned i = 0; i < cuts.size(); i++)
    {   
        thresholdScan* thrScan = new thresholdScan("Module thresholdScan: save results of threshold scan in histo plots", "Extract time at thresholds and calculate time difference, then save distribution for each threshold in a histo", "matchedLORs.lors.root", "output.root", thresholds, cuts[i]);
	manager.AddTask(thrScan);
    }   	

}



