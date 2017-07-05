//THIS IS A MAIN FILE FOR SOURCE ANALYSIS
#include "../../framework/JPetManager/JPetManager.h"
#include <utility>
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


using namespace std;

    struct PMT 
        {   
	  int id; 
          double p0; 
          double p1; 
        }; 
	
void signalAnalysis( JPetManager& manager, const std::vector<double> thresholds){
   // SDAcalib for offsets
   SDARecoOffsetsCalc* offsetsCalib = new SDARecoOffsetsCalc("Module SDARecoOffsetsCalc: calculating offsets of SDA signals", "Write SDA signals with offsets taken as a mean of noise points", "reco.sig.root", "offsets.sig.root");

   SDARecoChargeCalc* chargeCalc = new SDARecoChargeCalc("Module SDARecoChargeCalc: calculating area of SDA signals", "Write SDA signals with area calculated from first significant point, taking into account offset", "offsets.sig.root", "charge.sig.root");
	drawOffset* drawOffsetFor49 = new drawOffset("Module drawOffset: draw offset spectrum for selected PMT", "Saves PNG file of the draw spectrum", "charge.sig.root", "offsets.root",49);
	drawOffset* drawOffsetFor0= new drawOffset("Module drawOffset: draw offset spectrum for selected PMT", "Saves PNG file of the draw spectrum", "charge.sig.root", "offsets.root",0);
	drawOffset* drawOffsetFor44 = new drawOffset("Module drawOffset: draw offset spectrum for selected PMT", "Saves PNG file of the draw spectrum", "charge.sig.root", "offsets.root",44);
	drawOffset* drawOffsetFor45 = new drawOffset("Module drawOffset: draw offset spectrum for selected PMT", "Saves PNG file of the draw spectrum", "charge.sig.root", "offsets.root",45);
// 	manager.AddTask(drawOffsetFor49);
// 	manager.AddTask(drawOffsetFor0);
// 	manager.AddTask(drawOffsetFor44);
// 	manager.AddTask(drawOffsetFor45);

  drawAllOffsets* drawOffsets = new drawAllOffsets("Module drawAllOffsets: draw offsets for all PMT", "Finds heighest histo and plots it first, then the rest", "charge.sig.root", "offsets.root");
  drawAllCharges* drawCharges = new drawAllCharges("Module drawAllCharges: draw charges for all PMT", "Finds heighest histo and plots it first, then the rest", "charge.sig.root", "charges.root");


	
	
	SDARecoAmplitudeCalc* amplitudeCalc = new SDARecoAmplitudeCalc("Module SDARecoAmplitudeCalc: calculates signal amplitudes","","charge.sig.root","amps.sig.root");
	
	SDARecoTimeAtThr* timeAtThr = new SDARecoTimeAtThr("Module SDARecoTimeAtThr: calculates time at given threshold","Interpolates time between points as a line and check its crossing with threshold levels", "charge.sig.root","times.sig.root", thresholds);

	SDAMakePhysSignals* convertToPhys = new SDAMakePhysSignals("Module SDAMakePhysSignals: convert RecoSignals to Phys signals", "Produces phys signals with recosignal setted as their parents", "charge.sig.root", "afterPhysConvert.sig.root");  
	
  	manager.AddTask(offsetsCalib);
  	manager.AddTask(chargeCalc);
	//manager.AddTask(drawOffsets);
	//manager.AddTask(drawCharges);
//   	manager.AddTask(amplitudeCalc);
//    	manager.AddTask(timeAtThr);
   	manager.AddTask(convertToPhys);
// 
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

void amplitudeFitsFromLORs(JPetManager& manager)
{
  	SDALORFindAmplitudeConstant* findAmpConstants = new SDALORFindAmplitudeConstant("Module SDALORFindAmplitudeConstant: find calibration constants for amplitude spectra","Results are produced in fitResults.txt", "matchedLORs.lors.root", "out.root");
	SDALORExctractAmplitudes* exctractAmps = new SDALORExctractAmplitudes("","","matchedLORs.lors.root","out.root");
	
	manager.AddTask(findAmpConstants);
	manager.AddTask(exctractAmps);
}

void LORtimeCalib(JPetManager& manager)
{
      std::map<int, double> optimalThresholds;
//    optimalThresholds[86] = 100;
//    optimalThresholds[87] = 100;
     optimalThresholds[1] = 100;
     optimalThresholds[2] = 100;
    std::vector<double> thr;
    for(double t = 50; t< 501; t+=50)
	thr.push_back(t);
	
	SDALORCutOnEnergy* cutOnEnergy = new SDALORCutOnEnergy("Module SDAHitCutOnEnergy: cut all hits with energy lower than 200 keV", "Reads hit energies and cuts all with energy lower than threshold 200 keV", "calculatedEnergy.lors.root", "cuttedOnEnergy.lors.root", 200);
	SDALORCalculateTimeAtThr* calculateTimesAtThrs = new SDALORCalculateTimeAtThr("Module: SDALORCalculateTimeAtThr is calculating times at threshold before scan!","Reads file with LORs and calculates times at thr for signals", "matchedLORs.lors.root", "timeAtThrFilled.lors.root", thr);
	SDACalculateTimes* calculateTimesBeforeCalib = new SDACalculateTimes( "", "", "timeAtThrFilled.lors.root", "calculatedTimes.lors.root", optimalThresholds);
	SDAEstimateTimeCalib* estimateTimeOffsets = new SDAEstimateTimeCalib("","", "calculatedTimes.lors.root", "out.root", optimalThresholds);
	
//	std::pair<int,double> topStrip(2, 0.4363*1000.0);
//      std::pair<int,double> bottomStrip(1, 0.4396*1000.0);
 	std::pair<int,double> topStrip(72, +0.08849*1000.0);
        std::pair<int,double> bottomStrip(71, -0.1695*1000.0);
//	std::pair<int,double> topStrip(86, +0.0198*1000.0);
//        std::pair<int,double> bottomStrip(87, -0.1434*1000.0);
        std::vector< std::pair<int,double>> timeCalibration;
        timeCalibration.push_back(topStrip);
        timeCalibration.push_back(bottomStrip);
	
	SDADoTimeCalib* doTimeCalib = new SDADoTimeCalib("", "", "calculatedTimes.lors.root", "timeCalibrated.lors.root",  timeCalibration);
	SDACalculateTimes* calculateTimesAfterOffsetCalib = new SDACalculateTimes( "", "", "timeCalibrated.lors.root", "calculatedTimesAfterOffsetcalib.lors.root", optimalThresholds);
	SDAEstimateTimeCalib* estimateTimeOffsetsAfterCalib = new SDAEstimateTimeCalib("","", "calculatedTimesAfterOffsetcalib.lors.root", "out.root", optimalThresholds);
	SDAEstimateTOFCalib* estimateTOFBeforeCalib = new SDAEstimateTOFCalib("","", "calculatedTimesAfterOffsetcalib.lors.root", "out.root",110);
 	SDADoTOFCalib* doTOFCalib = new SDADoTOFCalib("", "", "calculatedTimesAfterOffsetcalib.lors.root", "tofCalibrated.lors.root", 6.441*1000.0); //for 30 cm 
//	SDADoTOFCalib* doTOFCalib = new SDADoTOFCalib("", "", "calculatedTimesAfterOffsetcalib.lors.root", "tofCalibrated.lors.root", 6.084*1000.0); //for 50 cm

	SDACalculateTimes* calculateTimesAfterTOFCalib = new SDACalculateTimes( "", "", "tofCalibrated.lors.root", "calculatedTimesAfterTOFcalib.lors.root", optimalThresholds);
	SDAEstimateTOFCalib* estimateTOFAfterCalib = new SDAEstimateTOFCalib("","", "calculatedTimesAfterTOFcalib.lors.root", "out.root",110);

//	manager.AddTask(cutOnEnergy);
// 	manager.AddTask(calculateTimesAtThrs);
	manager.AddTask(calculateTimesBeforeCalib );
	manager.AddTask(estimateTimeOffsets);
// 	manager.AddTask(doTimeCalib);
// 	manager.AddTask(calculateTimesAfterOffsetCalib);
//	manager.AddTask(estimateTimeOffsetsAfterCalib);
//	manager.AddTask(estimateTOFBeforeCalib);
// 	manager.AddTask(doTOFCalib);
// 	manager.AddTask(calculateTimesAfterTOFCalib);
//	manager.AddTask(estimateTOFAfterCalib);
}

void thresholdAndTimeScan(JPetManager& manager, std::vector<double> thresholds)
{
      std::vector<std::vector<double> >cuts;
    std::vector<double> cut;
    for(int i = 0; i < 8; i++)
    {   
        cut.push_back(50+i*50);
        cut.push_back(100+i*50);
        cuts.push_back(cut);
        cut.clear();
    }   
        cut.push_back(200);
        cuts.push_back(cut);
        cut.clear();
		
    SDALORCalculateTimeAtThr* calculateTimesAtThrs = new SDALORCalculateTimeAtThr("Module: SDALORCalculateTimeAtThr is calculating times at threshold before scan!","Reads file with LORs and calculates times at thr for signals", "matchedLORs.lors.root", "timeAtThrFilled.lors.root", thresholds);
    manager.AddTask(calculateTimesAtThrs);
    for(int unsigned i = 0; i < cuts.size(); i++)
    {   
        thresholdScan* thrScan = new thresholdScan("Module thresholdScan: save results of threshold scan in histo plots", "Extract time at thresholds and calculate time difference, then save distribution for each threshold in a histo", "timeAtThrFilled.lors.root", "output.root", thresholds, cuts[i]);
	manager.AddTask(thrScan);
    }   	

}

void TOTstudies(JPetManager& manager)
{
  	double TOTthreshold = 100;
	std::stringstream buf;
	buf << TOTthreshold;
	
 	SDALORCalculateTOT* changeChargeToTOTInPhysSignals = new SDALORCalculateTOT( "swappingChargeToTOT","swappingChargeToTOT", "matchedLORs.lors.root", "TOTswapped.lors.root", TOTthreshold);
	
	TOTVSQ* plotTOTvsQ = new TOTVSQ("TOTvsQplot","TOTvsQplot", "TOTswapped.lors.root", "out.root", buf.str());
	
	manager.AddTask(changeChargeToTOTInPhysSignals);
	manager.AddTask(plotTOTvsQ);

}

void walkCorrection(JPetManager& manager, std::map<int, double> optimalThresholds)
{
  	SDALORCalculateWalkCorrection* findWalkCorrection = new SDALORCalculateWalkCorrection("","", "calculatedTimesAfterTOFcalib.lors.root", "output.root", optimalThresholds);

    SDALORApplyWalkCorrectionToTOF* correctWalkInTOF = new SDALORApplyWalkCorrectionToTOF("applyingWalkToTOF","applyingWalkToTOF", "calculatedTimesAfterTOFcalib.lors.root", "tofWalkCalibrated.lors.root", 2875.28); // for 30 cm
//    SDALORApplyWalkCorrectionToTOF* correctWalkInTOF = new SDALORApplyWalkCorrectionToTOF("applyingWalkToTOF","applyingWalkToTOF", "calculatedTimesAfterTOFcalib.lors.root", "tofWalkCalibrated.lors.root", 2548.92);// for 50 cm
//    manager.AddTask(findWalkCorrection );
    manager.AddTask(correctWalkInTOF );
}

void outputForReco(JPetManager& manager)
{
   	SDACalculatePositions* calculatePositions = new SDACalculatePositions("", "", "tofWalkCalibrated.lors.root", "positionsCalculated.lors.root", 110.0, 12);

 	twoDRecoOutput* recoOutput = new twoDRecoOutput("output to reco", "output to reco", "positionsCalculated.lors.root", "dummy.root");	

// 	bpOutput* bpOut = new bpOutput("bp out", "bp out","tofCalibrated.lors.root", "dummy.root");	

	manager.AddTask(calculatePositions);
	manager.AddTask(recoOutput);
}

void testHowTOTCutInfluencesEDep(JPetManager& manager, const double TOTthreshold, const double TOTCut, const int PMTID)
{
  std::vector<double> thresholds;
  thresholds.push_back(10);
  for(int i = 30; i <400; i+=10)
  {
    thresholds.push_back((double)i);
  }
  for(int i = 450; i <1050; i+=50)
  {
    thresholds.push_back((double)i);
  }
  
    std::map<int, double> optimalThresholds;
    optimalThresholds[71] = 110;
    optimalThresholds[72] = 110;

  std::vector< std::pair<int,double>> dECalibration;
  std::pair<int,double> topStripE(72, 6.468);  
  std::pair<int,double> bottomStripE(71, 6.6);
  dECalibration.push_back(topStripE);
  dECalibration.push_back(bottomStripE);
    
  SDAMatchLORs* matchLORs = new SDAMatchLORs("Module SDAMatchLOR: save as LOR those hits which are in same TSlot and come from different scintillators", "Produces root file with JPetLORs from root file with JPetHits, JPetLORs are empty ", "matchedHits.hits.root", "matchedLORs.lors.root");
  
  SDALORCalculateEnergy* calculateEnergy = new SDALORCalculateEnergy("Module SDAHitCalculateEnergy: calculates energy of JPetHits", "Reads arithmetic sum of charges from JPetPhys and scales them with factor from SDAHitFindEnergyConstant module", "matchedLORs.lors.root", "calculatedEnergy.lors.root", dECalibration);
  
  SDALORCalculateTOT* changeChargeToTOTInPhysSignals = new SDALORCalculateTOT( "swappingChargeToTOT","swappingChargeToTOT", "calculatedEnergy.lors.root", "TOTswapped.lors.root", TOTthreshold);
  
  std::stringstream buf;
  buf << TOTthreshold;
  TOTVSQ* plotTOTvsQ = new TOTVSQ("TOTvsQplot","TOTvsQplot", "TOTswapped.lors.root", "out.root", buf.str());
  
  SDALORCutOnEnergy* cutOnEnergy = new SDALORCutOnEnergy("Module SDAHitCutOnEnergy: cut all hits with energy lower than 200 keV", "Reads hit energies and cuts all with energy lower than threshold 200 keV", "TOTswapped.lors.root", "cuttedOnEnergy.lors.root", 200);
  
  SDALORCutOnTOT* cutOnTOT = new SDALORCutOnTOT("cutting on TOT","cutting on TOT", "TOTswapped.lors.root", "TOTcut.lors.root", TOTCut, PMTID);
      
   // manager.AddTask(matchLORs);
//    manager.AddTask(calculateEnergy);
//    manager.AddTask(changeChargeToTOTInPhysSignals);
//   manager.AddTask(cutOnEnergy);
  manager.AddTask(cutOnTOT);
}


void scanThresholds(JPetManager& manager)
{
    std::map<int, double> optimalThresholds;
    std::vector<double> thr;
    for(double t = 10; t< 50; t+=10)
      thr.push_back(t);
    SDALORCalculateTimeAtThr* calculateTimesAtThrs = new SDALORCalculateTimeAtThr("Module: SDALORCalculateTimeAtThr is calculating times at threshold before scan!","Reads file with LORs and calculates times at thr for signals", "matchedLORs.lors.root", "timeAtThrFilled.lors.root", thr);
    manager.AddTask(calculateTimesAtThrs);
    for(double t = 10; t< 50; t+=10)
    {
	optimalThresholds[1] = t;
	optimalThresholds[2] = t;
	
	
	SDACalculateTimes* calculateTimesBeforeCalib = new SDACalculateTimes( "", "", "timeAtThrFilled.lors.root", "calculatedTimes.lors.root", optimalThresholds);
	SDAEstimateTimeCalib* estimateTimeOffsets = new SDAEstimateTimeCalib("","", "calculatedTimes.lors.root", "out.root", optimalThresholds);

	
	manager.AddTask(calculateTimesBeforeCalib );
	manager.AddTask(estimateTimeOffsets);
    }
}

int main(int argc, char* argv[])
{
  JPetManager& manager = JPetManager::GetManager();
  manager.ParseCmdLine(argc, argv);
  std::vector<double> thr;
  thr.push_back(110);
//  signalAnalysis(manager,thr);
//  hitsAnalysis(manager);
//  LORAnalysis(manager); 
  //LORtimeCalib(manager);
  scanThresholds(manager);
    std::map<int, double> optimalThresholds;
    optimalThresholds[71] = 110;	// for 30 cm long strips it is 110
    optimalThresholds[72] = 110;	// for 50 cm long strips it is 100
//    walkCorrection(manager, optimalThresholds);
//   outputForReco(manager);
  // run the analysis
  manager.Run();

}
