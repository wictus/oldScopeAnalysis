#include "./SDACalculatePositions.h"

SDACalculatePositions::SDACalculatePositions(const char* name, const char* title,
               const char* in_file_suffix, const char* out_file_suffix, const double threshold, 
	       const double positionCut)
: JPetCommonAnalysisModule( name, title, in_file_suffix, out_file_suffix), fCut(positionCut)
{
        fSelectedThreshold = threshold;
}

SDACalculatePositions::~SDACalculatePositions(){}

void SDACalculatePositions::begin()
{
}

void SDACalculatePositions::exec()
{
        fReader->getEntry(fEvent);

        JPetLOR& fLOR = dynamic_cast< JPetLOR& > ( fReader->getData() );

        JPetHit hit = fLOR.getFirstHit();
        hit.setPosAlongStrip( hit.getTimeDiff()/2.0 * 12.6/1000.0  );
        fLOR.setFirstHit( hit );

        hit = fLOR.getSecondHit();
        hit.setPosAlongStrip( hit.getTimeDiff()/2.0 * 12.6/1000.0  );
        fLOR.setSecondHit( hit );
        
	if( fLOR.getFirstHit().getPosAlongStrip() > fCut )
	{
	  fEvent++;
	  return;
	}
	
	fWriter->write(fLOR);
 
        fEvent++;
}

void SDACalculatePositions::end()
{
}

