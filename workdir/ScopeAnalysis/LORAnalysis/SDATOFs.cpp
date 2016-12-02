#include "./SDATOFs.h"
#include "sstream"
#include <stdlib.h>
#include "fstream"


ClassImp(SDATOFs);

SDATOFs::SDATOFs(const char* name, const char* title,
                 const char* in_file_suffix, const char* out_file_suffix) :
  JPetCommonAnalysisModule(name, title, in_file_suffix, out_file_suffix)
{
  setVersion(MODULE_VERSION);
}

SDATOFs::~SDATOFs()
{
}

void SDATOFs::begin()
{
 
}

void SDATOFs::exec()
{

  // Get a LOR 
  fReader->getEntry(fEvent);
  const JPetLOR& fullEvent = (JPetLOR&) fReader->getData();

  JPetLOR newFullEvent = fullEvent;

  double timeTop = fullEvent.getFirstHit().getTime();
  double timeBottom = fullEvent.getSecondHit().getTime();
  newFullEvent.setTimeDiff( ( timeBottom - timeTop ) );

  fWriter->write(newFullEvent);
// increase event counter
  fEvent++;
}


void SDATOFs::end()
{
}
