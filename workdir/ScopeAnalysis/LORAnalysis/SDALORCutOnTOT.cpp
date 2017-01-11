#include "./SDALORCutOnTOT.h"

ClassImp(SDALORCutOnTOT);

SDALORCutOnTOT::SDALORCutOnTOT(const char* name, const char* title,
                               const char* in_file_suffix, const char* out_file_suffix, 
			       const double thresholdValue, const int PMTID):
  JPetCommonAnalysisModule(name, title, in_file_suffix, out_file_suffix), 
  fTOTThreshold(thresholdValue), fPMTID(PMTID)
{
  setVersion(MODULE_VERSION);
}

SDALORCutOnTOT::~SDALORCutOnTOT()
{
}

void SDALORCutOnTOT::begin()
{
	fAboveThreshold = 0;
}

void SDALORCutOnTOT::exec()
{

  fReader->getEntry(fEvent);
  const JPetLOR lor = (JPetLOR&) fReader->getData();
  performCut(lor);
  
// increase event counter
  fEvent++;
}


void SDALORCutOnTOT::end()
{
	INFO(Form("%f  events were above threshold", (double)fAboveThreshold/(double)fEventNb 	));
	std::cout << "Events above the threshold TOT: " << fAboveThreshold << std::endl;
}

void SDALORCutOnTOT::performCut(const JPetLOR& lor)
{
  JPetPhysSignal signal = findSignalToCut(lor);
  if(signal.getPhe() > fTOTThreshold)
  {
    fWriter->write(lor);
    fAboveThreshold++;
  }
}

JPetPhysSignal SDALORCutOnTOT::findSignalToCut(const JPetLOR& lor)
{
  JPetPhysSignal phys;
  if(lor.getFirstHit().getSignalA().getRecoSignal().getPM().getID() == fPMTID)
  {std::cout << "found PMT at signalA first hit\n";
    return lor.getFirstHit().getSignalA();
  }
  
  else if(lor.getFirstHit().getSignalB().getRecoSignal().getPM().getID() == fPMTID)
    {std::cout << "found PMT at signalB first hit\n";
    return lor.getFirstHit().getSignalB();
    }
  
  else if(lor.getSecondHit().getSignalA().getRecoSignal().getPM().getID() == fPMTID)
    {std::cout << "found PMT at signalA second hit\n";
    return lor.getSecondHit().getSignalA();
    }
  
  else if(lor.getSecondHit().getSignalB().getRecoSignal().getPM().getID() == fPMTID)
    {std::cout << "found PMT at signalB second hit\n";
    return lor.getSecondHit().getSignalB();
    }
  return phys;
}

