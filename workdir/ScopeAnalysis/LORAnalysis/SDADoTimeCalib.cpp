#include "./SDADoTimeCalib.h"

SDADoTimeCalib::SDADoTimeCalib(const char* name, const char* title,
               const char* in_file_suffix, const char* out_file_suffix, std::vector< std::pair <int, double>> timeCalib) : JPetCommonAnalysisModule( name, title, in_file_suffix, out_file_suffix)
{
	fCalibration = timeCalib;
}

SDADoTimeCalib::~SDADoTimeCalib(){}

void SDADoTimeCalib::begin()
{
}

void SDADoTimeCalib::exec()
{
	fReader->getEntry(fEvent);

	JPetLOR& fLOR = dynamic_cast< JPetLOR& > ( fReader->getData() );

	std::map<float, float>& measuredTimes = const_cast <JPetRecoSignal&> (fLOR.getSecondHit().getSignalA().getRecoSignal() ).getRecoTimesAtThreshold();

  for(unsigned int i = 0; i < fCalibration.size(); i++)
  {
        if( fLOR.getFirstHit().getScintillator().getID() == fCalibration[i].first )
        {
	   for(auto& point: measuredTimes)
	   {
		double threshold = point.first;
		double time = fLOR.getFirstHit().getSignalB().getRecoSignal().getRecoTimeAtThreshold( threshold );
// 		std::cout << point.first << " " << point.second << std::endl;
		if( 0 != time ){
			time+=fCalibration[i].second;
	   		const_cast <JPetRecoSignal&>(fLOR.getFirstHit().getSignalB().getRecoSignal()).setRecoTimeAtThreshold( threshold , time );
		}
	   }
        }
        else if( fLOR.getSecondHit().getScintillator().getID() == fCalibration[i].first )
        {
	   for(auto& point: measuredTimes)
	   {
		double threshold = point.first;
	   	double time = fLOR.getSecondHit().getSignalB().getRecoSignal().getRecoTimeAtThreshold( threshold );
		if( 0 != time) {
			time+=fCalibration[i].second;
			const_cast <JPetRecoSignal&>(fLOR.getSecondHit().getSignalB().getRecoSignal()).setRecoTimeAtThreshold( threshold , time );
		}
	   }
        }
        else
        {
                WARNING( Form("DID NOT FIND SCINTILLATOR IN GIVEN CALIBRATION VECTOR!")  );
        }
  }
	fWriter->write(fLOR);

	fEvent++;
}

void SDADoTimeCalib::end()
{
  std::cout<<fEvent<<std::endl;
}
