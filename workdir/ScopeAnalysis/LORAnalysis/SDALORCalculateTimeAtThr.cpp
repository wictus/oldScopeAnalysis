#include "./SDALORCalculateTimeAtThr.h"
#include "../JPetRecoSignalTools/JPetRecoSignalTools.h"

ClassImp(SDALORCalculateTimeAtThr);

SDALORCalculateTimeAtThr::SDALORCalculateTimeAtThr( const char* name, const char* title, const char* in_file_suffix, 
				const char* out_file_suffix, const std::vector<double>& givenThresholds ) : 
				JPetCommonAnalysisModule( name, title, in_file_suffix, out_file_suffix ){
	fThresholds = givenThresholds;
}

SDALORCalculateTimeAtThr::~SDALORCalculateTimeAtThr(){ }

void SDALORCalculateTimeAtThr::begin(){ }

void SDALORCalculateTimeAtThr::exec(){

	fReader->getEntry(fEvent);
	const JPetLOR& inputLOR = (JPetLOR&) fReader->getData();

	fCurrentLOR = inputLOR;
// 	if( inputLOR.getFirstHit().getSignalA().getPhe() > 88 && inputLOR.getFirstHit().getSignalB().getPhe() > 33)
// 	{
		for(unsigned int j = 0; j < fThresholds.size(); j++){
                     double threshold =  fThresholds[j];
		     double time = JPetRecoSignalTools::calculateTimeAtThreshold( fCurrentLOR.getFirstHit().getSignalA().getRecoSignal(), threshold );
			if( JPetRecoSignalTools::ERRORS::badTimeAtThr != time ) 
			     const_cast <JPetRecoSignal&>(fCurrentLOR.getFirstHit().getSignalA().getRecoSignal()).setRecoTimeAtThreshold( threshold , time );
				

		     time = JPetRecoSignalTools::calculateTimeAtThreshold( fCurrentLOR.getFirstHit().getSignalB().getRecoSignal(), threshold );
			if( JPetRecoSignalTools::ERRORS::badTimeAtThr != time ) 
		     const_cast <JPetRecoSignal&>(fCurrentLOR.getFirstHit().getSignalB().getRecoSignal()).setRecoTimeAtThreshold( threshold , time );

		     time = JPetRecoSignalTools::calculateTimeAtThreshold( fCurrentLOR.getSecondHit().getSignalA().getRecoSignal(), threshold );
			if( JPetRecoSignalTools::ERRORS::badTimeAtThr != time ) 
		     const_cast <JPetRecoSignal&>(fCurrentLOR.getSecondHit().getSignalA().getRecoSignal()).setRecoTimeAtThreshold( threshold , time );

		     time = JPetRecoSignalTools::calculateTimeAtThreshold( fCurrentLOR.getSecondHit().getSignalB().getRecoSignal(), threshold );
			if( JPetRecoSignalTools::ERRORS::badTimeAtThr != time ) 
		     const_cast <JPetRecoSignal&>(fCurrentLOR.getSecondHit().getSignalB().getRecoSignal()).setRecoTimeAtThreshold( threshold , time );
		}
// 	}
	fWriter->write( fCurrentLOR );

	fEvent++;
}

void SDALORCalculateTimeAtThr::end(){ }
