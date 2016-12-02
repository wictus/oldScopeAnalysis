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

using namespace std;

    struct PMT 
        {   
                    int id; 
                            double p0; 
                                    double p1; 
                                        }; 

int main(int argc, char* argv[])
{
  JPetManager& manager = JPetManager::GetManager();
  manager.ParseCmdLine(argc, argv);

  // SDAcalib for offsets
   SDARecoOffsetsCalc* offsetsCalib = new SDARecoOffsetsCalc("Module SDARecoOffsetsCalc: calculating offsets of SDA signals", "Write SDA signals with offsets taken as a mean of noise points", "reco.sig.root", "offsets.sig.root");

   SDARecoChargeCalc* chargeCalc = new SDARecoChargeCalc("Module SDARecoChargeCalc: calculating area of SDA signals", "Write SDA signals with area calculated from first significant point, taking into account offset", "offsets.sig.root", "charge.sig.root");
/*	drawOffset* drawOffsetFor49 = new drawOffset("Module drawOffset: draw offset spectrum for selected PMT", "Saves PNG file of the draw spectrum", "charge.sig.root", "offsets.root",49);
	drawOffset* drawOffsetFor.= new drawOffset("Module drawOffset: draw offset spectrum for selected PMT", "Saves PNG file of the draw spectrum", "charge.sig.root", "offsets.root",.;
	drawOffset* drawOffsetFor44 = new drawOffset("Module drawOffset: draw offset spectrum for selected PMT", "Saves PNG file of the draw spectrum", "charge.sig.root", "offsets.root",44;
	drawOffset* drawOffsetFor45 = new drawOffset("Module drawOffset: draw offset spectrum for selected PMT", "Saves PNG file of the draw spectrum", "charge.sig.root", "offsets.root",45);
//	manager.AddTask(drawOffsetFor49);
//	manager.AddTask(drawOffsetFor.;
//	manager.AddTask(drawOffsetFor44);
//	manager.AddTask(drawOffsetFor45);
*/
  drawAllOffsets* drawOffsets = new drawAllOffsets("Module drawAllOffsets: draw offsets for all PMT", "Finds heighest histo and plots it first, then the rest", "charge.sig.root", "offsets.root");
  drawAllCharges* drawCharges = new drawAllCharges("Module drawAllCharges: draw charges for all PMT", "Finds heighest histo and plots it first, then the rest", "charge.sig.root", "charges.root");

/*
    std::vector<PMT> PMTs;
    PMT P49 = { 49, 14.1E-4, 7. };
    PMTs.push_back( P49 );
    PMT P.= { . 118.8E-5, 7.24 };
    PMTs.push_back( P49 );
    PMT P44 = { 44, 24.4E-4, 6.557 };
    PMTs.push_back( P49 );
    PMT P45 = { 45, 15.2E-4, 6.88 };
    PMTs.push_back( P49 );

    for(unsigned int i =. i < PMTs.size(); i++ )
        {
            addCalibration* addCurve = new addCalibration("Module addCalibration: add gain curve parameters to selected PMT",
            "Finds PMT in paramBank and sets its HVgain to given values", "charge.sig.root", "afterGainCalib.sig.root",
            PMTs[i].id, PMTs[i].. PMTs[i].p1);
            manager.AddTask(addCurve);
        }
*/
	
	std::vector<double> thresholds;
	thresholds.push_back(10);
/*	for(int i = 30; i <400; i+=10)
	{
		thresholds.push_back((double)i);
	}
	for(int i = 450; i <1050; i+=50)
	{
		thresholds.push_back((double)i);
	}
*/	
	SDARecoAmplitudeCalc* amplitudeCalc = new SDARecoAmplitudeCalc("Module SDARecoAmplitudeCalc: calculates signal amplitudes","","charge.sig.root","amps.sig.root");
	
	SDARecoTimeAtThr* timeAtThr = new SDARecoTimeAtThr("Module SDARecoTimeAtThr: calculates time at given threshold","Interpolates time between points as a line and check its crossing with threshold levels", "amps.sig.root","times.sig.root", thresholds);

	SDAMakePhysSignals* convertToPhys = new SDAMakePhysSignals("Module SDAMakePhysSignals: convert RecoSignals to Phys signals", "Produces phys signals with recosignal setted as their parents", "times.sig.root", "afterPhysConvert.sig.root");

	SDAMatchHits* matchHits = new SDAMatchHits("Module SDAMatchHits: save as hits those events which have exactly two signals in one TSlot", "Produces root file with JPetHits from root file with JPetPhysSignals ","afterPhysConvert.sig.root", "matchedHits.hits.root");

	std::vector< std::pair<int,double>> dECalibration;
	std::pair<int,double> topStripE(72, 6.468);  
        std::pair<int,double> bottomStripE(71, 6.6);
	dECalibration.push_back(topStripE);
	dECalibration.push_back(bottomStripE);

	SDAHitFindEnergyConstant* makeFit = new SDAHitFindEnergyConstant("Module SDAHitFindEnergyConstant: finds energy scaling factor", "Reads charges from both PMTs forming a Hit, takes avereage and fits to the charge spectrum MC simulation", "matchedHits.hits.root", "foundEnergy.hits.root");

	SDAHitCalculateEnergy* calculateEnergy = new SDAHitCalculateEnergy("Module SDAHitCalculateEnergy: calculates energy of JPetHits", "Reads arithmetic sum of charges from JPetPhys and scales them with factor from SDAHitFindEnergyConstant module", "foundEnergy.hits.root", "calculatedEnergy.hits.root");

	SDAMatchLORs* matchLORs = new SDAMatchLORs("Module SDAMatchLOR: save as LOR those hits which are in same TSlot and come from different scintillators", "Produces root file with JPetLORs from root file with JPetHits, JPetLORs are empty ", "matchedHits.hits.root", "matchedLORs.lors.root");
	
	SDALORFindAmplitudeConstant* findAmpConstants = new SDALORFindAmplitudeConstant("Module SDALORFindAmplitudeConstant: find calibration constants for amplitude spectra","Results are produced in fitResults.txt", "matchedLORs.lors.root", "out.root");

	SDALORExctractAmplitudes* exctractAmps = new SDALORExctractAmplitudes("","","matchedLORs.lors.root","out.root");
//FOR SOURCES ONLY
//	SDALORCalculateEnergy* calculateEnergy = new SDALORCalculateEnergy("Module SDAHitCalculateEnergy: calculates energy of JPetHits", "Reads arithmetic sum of charges from JPetPhys and scales them with factor from SDAHitFindEnergyConstant module", "matchedLORs.lors.root", "calculatedEnergy.lors.root", dECalibration);




//   	manager.AddTask(offsetsCalib);
//   	manager.AddTask(chargeCalc);
//	manager.AddTask(drawOffsets);
//	manager.AddTask(drawCharges);
//    	manager.AddTask(amplitudeCalc);
//    	manager.AddTask(timeAtThr);
//    	manager.AddTask(convertToPhys);
//   	manager.AddTask(matchHits);
// 	manager.AddTask(makeFit);
// 	manager.AddTask(calculateEnergy);
//   	manager.AddTask(matchLORs);
//	manager.AddTask(findAmpConstants);
//	manager.AddTask(exctractAmps);
	
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
//    manager.AddTask(calculateTimesAtThrs);
    for(int i = 0; i < cuts.size(); i++)
    {   
        thresholdScan* thrScan = new thresholdScan("Module thresholdScan: save results of threshold scan in histo plots", "Extract time at thresholds and calculate time difference, then save distribution for each threshold in a histo", "timeAtThrFilled.lors.root", "output.root", thresholds, cuts[i]);
//	manager.AddTask(thrScan);
    }   	
    
    std::map<int, double> optimalThresholds;
    optimalThresholds[71] = 110;
    optimalThresholds[72] = 110;
      
	SDALORCutOnEnergy* cutOnEnergy = new SDALORCutOnEnergy("Module SDAHitCutOnEnergy: cut all hits with energy lower than 200 keV", "Reads hit energies and cuts all with energy lower than threshold 200 keV", "matchedLORs.lors.root", "cuttedOnEnergy.lors.root", 200);
	
	SDACalculateTimes* calculateTimesBeforeCalib = new SDACalculateTimes( "", "", "cuttedOnEnergy.lors.root", "calculatedTimes.lors.root", optimalThresholds);
	SDAEstimateTimeCalib* estimateTimeOffsets = new SDAEstimateTimeCalib("","", "calculatedTimes.lors.root", "out.root", optimalThresholds);
	
//	std::pair<int,double> topStrip(2, 0.4363*1000.0);
//      std::pair<int,double> bottomStrip(1, 0.4396*1000.0);
//	std::pair<int,double> topStrip(72, +0.08849*1000.0);
//        std::pair<int,double> bottomStrip(71, -0.1695*1000.0);
	std::pair<int,double> topStrip(72, +0.09357*1000.0);
        std::pair<int,double> bottomStrip(71, -0.1684*1000.0);
        std::vector< std::pair<int,double>> timeCalibration;
        timeCalibration.push_back(topStrip);
        timeCalibration.push_back(bottomStrip);
	
	SDADoTimeCalib* doTimeCalib = new SDADoTimeCalib("", "", "calculatedTimes.lors.root", "timeCalibrated.lors.root",  timeCalibration);
	SDACalculateTimes* calculateTimesAfterOffsetCalib = new SDACalculateTimes( "", "", "timeCalibrated.lors.root", "calculatedTimesAfterOffsetcalib.lors.root", optimalThresholds);
	SDAEstimateTimeCalib* estimateTimeOffsetsAfterCalib = new SDAEstimateTimeCalib("","", "calculatedTimesAfterOffsetcalib.lors.root", "out.root", optimalThresholds);
	
	SDADoTOFCalib* doTOFCalib = new SDADoTOFCalib("", "", "calculatedTimesAfterOffsetcalib.lors.root", "tofCalibrated.lors.root", 6.44132*1000.0);

	SDACalculateTimes* calculateTimesAfterTOFCalib = new SDACalculateTimes( "", "", "tofCalibrated.lors.root", "calculatedTimesAfterTOFcalib.lors.root", optimalThresholds);
	
	double TOTthreshold = 100;
	std::stringstream buf;
	buf << TOTthreshold;
	
	SDALORCalculateTOT* changeChargeToTOTInPhysSignals = new SDALORCalculateTOT( "swappingChargeToTOT","swappingChargeToTOT", "matchedLORs.lors.root", "TOTswapped.lors.root", TOTthreshold);
	
	TOTVSQ* plotTOTvsQ = new TOTVSQ("TOTvsQplot","TOTvsQplot", "TOTswapped.lors.root", "out.root", buf.str());
	
	SDALORCalculateWalkCorrection* findWalkCorrection = new SDALORCalculateWalkCorrection("","", "calculatedTimesAfterTOFcalib.lors.root", "output.root", optimalThresholds);

	SDALORApplyWalkCorrectionToTOF* correctWalkInTOF = new SDALORApplyWalkCorrectionToTOF("applyingWalkToTOF","applyingWalkToTOF", "TOTswapped.lors.root", "tofWalkCalibrated.lors.root", 1.22073e+06);
	
	SDACalculatePositions* calculatePositions = new SDACalculatePositions("", "", "calculatedTimes.lors.root", "positionsCalculated.lors.root", 110.0, 200);

	twoDRecoOutput* recoOutput = new twoDRecoOutput("output to reco", "output to reco", "cuttedOnEnergy.lors.root", "dummy.root");	

	bpOutput* bpOut = new bpOutput("bp out", "bp out","tofCalibrated.lors.root", "dummy.root");	

//	manager.AddTask(cutOnEnergy);	
// 	manager.AddTask(calculateTimesBeforeCalib);
//	manager.AddTask(estimateTimeOffsets);
// 	manager.AddTask(doTimeCalib);
//	manager.AddTask(calculateTimesAfterOffsetCalib);
//	manager.AddTask(estimateTimeOffsetsAfterCalib);
//	manager.AddTask(doTOFCalib);
//	manager.AddTask(calculateTimesAfterTOFCalib);
//	manager.AddTask(changeChargeToTOTInPhysSignals);


//	manager.AddTask(changeChargeToTOTInPhysSignals);
//	manager.AddTask(findWalkCorrection);
//	manager.AddTask(correctWalkInTOF);
	manager.AddTask(plotTOTvsQ);
//	manager.AddTask(bpOut);
//	manager.AddTask(calculatePositions);

//	manager.AddTask(recoOutput);

  // run the analysis
  manager.Run();

}
