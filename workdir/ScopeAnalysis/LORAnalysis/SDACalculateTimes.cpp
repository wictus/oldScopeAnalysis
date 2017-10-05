#include "./SDACalculateTimes.h"

SDACalculateTimes::SDACalculateTimes(const char* name, const char* title, const char* in_file_suffix, const char* out_file_suffix, std::map<int, double> thresholds) : JPetCommonAnalysisModule( name, title, in_file_suffix, out_file_suffix)
{
        fSelectedThreshold = thresholds;
}

SDACalculateTimes::~SDACalculateTimes(){}

void SDACalculateTimes::begin()
{
}

void SDACalculateTimes::exec()
{
        fReader->getEntry(fEvent);

        JPetLOR& fLOR = dynamic_cast< JPetLOR& > ( fReader->getData() );

	
        double timeLeft = fLOR.getFirstHit().getSignalA().getRecoSignal().getRecoTimeAtThreshold( fSelectedThreshold[ fLOR.getFirstHit().getSignalA().getRecoSignal().getPM().getScin().getID() ]	);
        double timeRight = fLOR.getFirstHit().getSignalB().getRecoSignal().getRecoTimeAtThreshold( fSelectedThreshold [ fLOR.getFirstHit().getSignalB().getRecoSignal().getPM().getScin().getID() ] );
	
	if( 0 == timeLeft || 0 == timeRight )
	{
	  WARNING(Form("Omitting event %d",fEvent));
	  fEvent++;
	  return;
	}
	
        JPetHit hit = fLOR.getFirstHit();
        hit.setTime( (timeLeft + timeRight) / 2.0 );
	hit.setTimeDiff( timeRight - timeLeft );
        fLOR.setFirstHit( hit );

        timeLeft = fLOR.getSecondHit().getSignalA().getRecoSignal().getRecoTimeAtThreshold( fSelectedThreshold[  fLOR.getSecondHit().getSignalA().getRecoSignal().getPM().getScin().getID() ] );
        timeRight = fLOR.getSecondHit().getSignalB().getRecoSignal().getRecoTimeAtThreshold( fSelectedThreshold[ fLOR.getSecondHit().getSignalB().getRecoSignal().getPM().getScin().getID() ] );
	
	if( 0 == timeLeft || 0 == timeRight )
	{
	  WARNING(Form("Omitting event %d",fEvent));
	  fEvent++;
	  return;
	}
	
        hit = fLOR.getSecondHit();
        hit.setTime( (timeLeft + timeRight) / 2.0 );
	hit.setTimeDiff( timeRight - timeLeft );
        fLOR.setSecondHit( hit );

	fLOR.setTimeDiff(  fLOR.getFirstHit().getTime() - fLOR.getSecondHit().getTime() );
        fWriter->write(fLOR);

        fEvent++;
}

void SDACalculateTimes::end()
{
}

