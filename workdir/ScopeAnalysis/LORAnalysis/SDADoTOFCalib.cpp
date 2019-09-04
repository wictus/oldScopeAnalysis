#include "./SDADoTOFCalib.h"

SDADoTOFCalib::SDADoTOFCalib(const char* name, const char* title,
               const char* in_file_suffix, const char* out_file_suffix, double timeCalib) : JPetCommonAnalysisModule( name, title, in_file_suffix, out_file_suffix)
{
	fCalibration = timeCalib;
}

SDADoTOFCalib::~SDADoTOFCalib(){}

void SDADoTOFCalib::begin()
{
}

void SDADoTOFCalib::exec()
{
	fReader->getEntry(fEvent);

	JPetLOR& fLOR = dynamic_cast< JPetLOR& > ( fReader->getData() );

	std::map<float, float>& measuredTimes = const_cast <JPetRecoSignal&> (fLOR.getSecondHit().getSignalB().getRecoSignal() ).getRecoTimesAtThreshold();

	for(auto& point: measuredTimes)
	{
		double threshold = point.first;
		
		double time = fLOR.getSecondHit().getSignalB().getRecoSignal().getRecoTimeAtThreshold( threshold );

		if( 0 != time ){
			time+=fCalibration;}

		const_cast <JPetRecoSignal&>(fLOR.getSecondHit().getSignalB().getRecoSignal()).setRecoTimeAtThreshold( threshold , time );

		time = fLOR.getSecondHit().getSignalA().getRecoSignal().getRecoTimeAtThreshold( threshold );

		if( 0 != time ){
			time+=fCalibration;}

		const_cast <JPetRecoSignal&>(fLOR.getSecondHit().getSignalA().getRecoSignal()).setRecoTimeAtThreshold( threshold , time );

	}

	fWriter->write(fLOR);

	fEvent++;
}

void SDADoTOFCalib::end()
{
}

