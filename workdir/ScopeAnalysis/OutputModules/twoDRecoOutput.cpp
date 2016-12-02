#include "./twoDRecoOutput.h"
#include "sstream"
#include <stdlib.h>
#include "fstream"
#include <math.h>

ClassImp(twoDRecoOutput);

twoDRecoOutput::twoDRecoOutput(const char* name, const char* title,
                 const char* in_file_suffix, const char* out_file_suffix) :
  JPetCommonAnalysisModule(name, title, in_file_suffix, out_file_suffix)
{
  setVersion(MODULE_VERSION);
}

twoDRecoOutput::~twoDRecoOutput()
{
}

void twoDRecoOutput::begin()
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

void twoDRecoOutput::exec()
{

  // Get a LOR 
  fReader->getEntry(fEvent);
  const JPetLOR& fullEvent = (JPetLOR&) fReader->getData();

  JPetLOR newFullEvent = fullEvent;

  fZ1.push_back(newFullEvent.getFirstHit().getPosAlongStrip()  );
  fZ2.push_back(newFullEvent.getSecondHit().getPosAlongStrip() );
  fTOF.push_back( newFullEvent.getTimeDiff() );

// increase event counter
  fEvent++;
}


void twoDRecoOutput::end()
{
        //opening file with and setting high precision
  outFile.precision(20);

  for (unsigned int i = 0; i < fTOF.size(); i++) {
    outFile << fZ1[i] << "  " << fZ2[i] << "   " <<  fTOF[i] <<  std::endl;
  }

  double zTopMean = 0;
  double zBottomMean = 0;

  for( unsigned int i = 0; i < fZ1.size(); i++)
  {
	zTopMean+=fZ1[i];
  }
  zTopMean/=fZ1.size();
 
  for( unsigned int i = 0; i < fZ2.size(); i++)
  {
	zBottomMean+=fZ2[i];
  }
  zBottomMean/=fZ2.size();

  double zTopDev = 0;
  for( unsigned int i = 0; i < fZ1.size(); i++)
  {
	zTopDev+= pow( fZ1[i] - zTopMean , 2.0);
  }
  zTopDev = sqrt(zTopDev);

  double zBottomDev = 0;
  for( unsigned int i = 0; i < fZ2.size(); i++)
  {
	zBottomDev+= pow( fZ2[i] - zBottomMean , 2.0);
  }

  zBottomDev = sqrt(zBottomDev);

  double correlation = 0;
  for(unsigned int i = 0; i < fZ1.size(); i++ )
  {
	correlation += (fZ1[i] - zTopMean)*(fZ2[i] - zBottomMean);
  }
  correlation /= zTopDev;
  correlation /= zBottomDev;

  std::cout<< "mean top/bottom for: " << fHeader->getSourcePosition() << "  is equal to: " << zTopMean << "/" << zBottomMean << std::endl;
  std::cout<< "std dev top/bottom for: " << fHeader->getSourcePosition() << "  is equal to: " << zTopDev/ sqrt(fZ1.size()) << "/" << zBottomDev/ sqrt(fZ2.size()) << std::endl;
  std::cout<< "correlation for: " << fHeader->getSourcePosition() << "  is equal to: " << correlation << std::endl << std::endl;

  outFile.close();
}

