#include "./bossOutput.h"
#include "../JPetRecoSignalTools/JPetRecoSignalTools.h"
#include "../../../framework/JPetManager/JPetManager.h"

ClassImp(bossOutput);

//standard constructor from AnalysisModule
bossOutput::bossOutput(const char* name, const char* title,
                   const char* in_file_suffix, const char* out_file_suffix)
                   : JPetCommonAnalysisModule(name, title, in_file_suffix, out_file_suffix)
{
  setVersion(MODULE_VERSION);
}

//no specific destructor needed
bossOutput::~bossOutput()
{
}

void bossOutput::begin()
{
  std::string filePath = (std::string)fHeader->getBaseFileName();
  filePath = filePath.substr(filePath.find_last_of("/")+1 );
  filePath = filePath.substr(0, filePath.find_last_of(".") );
  double position = fHeader->getSourcePosition();
  std::stringstream buf;
  buf << position;
  filePath+="_"+buf.str();
  filePath+=".txt";
  INFO(Form("Output file will be produced in: %s", filePath.c_str()) );
  outFile.open(filePath.c_str());

}

void bossOutput::exec()
{

  // Take next entry 
  fReader->getEntry(fEvent);

  // Cast data from the entry into JPetLOR
  const JPetLOR& lor = (JPetLOR&) fReader->getData();

  // Extract signals
  const JPetRecoSignal& signalFirst = lor.getFirstHit().getSignalA().getRecoSignal();
  const JPetRecoSignal& signalSecond = lor.getFirstHit().getSignalB().getRecoSignal();
  const JPetRecoSignal& signalThird = lor.getSecondHit().getSignalA().getRecoSignal();
  const JPetRecoSignal& signalFourth = lor.getSecondHit().getSignalB().getRecoSignal();

  // Save times from signals to file
	outFile << "TIMES: " << "\t";
  for( unsigned int thresholdValue = 10; thresholdValue < 400; thresholdValue +=10)
  {
	outFile << signalFirst.getRecoTimeAtThreshold( (double)thresholdValue) << "\t" <<  signalSecond.getRecoTimeAtThreshold( (double)thresholdValue) << "\t" << signalThird.getRecoTimeAtThreshold( (double)thresholdValue) << "\t" << signalFourth.getRecoTimeAtThreshold( (double)thresholdValue) << "\t";
  }
  
  for( unsigned int thresholdValue = 400; thresholdValue < 1001; thresholdValue +=50)
  {
	outFile << signalFirst.getRecoTimeAtThreshold( (double)thresholdValue) << "\t" <<  signalSecond.getRecoTimeAtThreshold( (double)thresholdValue) << "\t" << signalThird.getRecoTimeAtThreshold( (double)thresholdValue) << "\t" << signalFourth.getRecoTimeAtThreshold( (double)thresholdValue) << "\t";
  }

  // Extract hits
  const JPetHit& hitFirst = lor.getFirstHit();
  const JPetHit& hitSecond = lor.getSecondHit();

  // Save charges energy depositions
  outFile << "Energy dep: " << "\t";
  outFile << hitFirst.getEnergy() << "\t" << hitSecond.getEnergy() << "\t";

  // Save charges
  outFile << "Charges: " << "\t";
  outFile << signalFirst.getCharge() << "\t" <<signalSecond.getCharge() << "\t" << signalThird.getCharge() << "\t"  << signalFourth.getCharge() << "\t" ;

  // Save scale factor (temporary saved as QualityOfEnergy)
  outFile << "Energy scale factor: " << "\t";
  outFile << hitFirst.getQualityOfTime() << "\t" << hitSecond.getQualityOfTime() << "\t";

  // Save energy resolution (temporary saved as QualityOfTime)
  outFile << "Energy resolution: " << "\t";
  outFile << hitFirst.getQualityOfEnergy() << "\t" << hitSecond.getQualityOfEnergy() << "\t";

  outFile << std::endl;

  // increase event counter
  fEvent++;
}

void bossOutput::end()
{
	outFile.close();
}

