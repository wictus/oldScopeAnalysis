#include "TOTVSQ.h"
#include <boost/concept_check.hpp>

TOTVSQ::TOTVSQ(const char* name, const char* title, const char* in_file_suffix, const char* out_file_suffix, const std::string threshold)
: JPetCommonAnalysisModule(name, title, in_file_suffix, out_file_suffix), fThreshold( threshold )
{

}

TOTVSQ::~TOTVSQ()
{

}

void TOTVSQ::begin()
{
  const JPetParamBank& bank = getParamBank();
  
  for(int i = 0; i < bank.getPMsSize(); i++ )
  {
    std::vector<std::pair<double,double>> dummy;
    fValuesPerPMT[ bank.getPM(i).getID() ] = dummy;
  }
}

void TOTVSQ::exec()
{
  fReader->getEntry(fEvent);
  const JPetLOR& LOR = (JPetLOR&) fReader->getData();

  int ID = LOR.getFirstHit().getSignalA().getRecoSignal().getPM().getID();
  double TOT = LOR.getFirstHit().getSignalA().getPhe();
  double Q = LOR.getFirstHit().getSignalA().getRecoSignal().getCharge();
  std::pair< double , double > point = std::make_pair(Q, TOT);
  fValuesPerPMT[ ID ].push_back(point);
  
 
  ID= LOR.getFirstHit().getSignalB().getRecoSignal().getPM().getID();
  TOT = LOR.getFirstHit().getSignalB().getPhe();
  Q = LOR.getFirstHit().getSignalB().getRecoSignal().getCharge();
  point = std::make_pair(Q, TOT);
  fValuesPerPMT[ ID ].push_back(point);
  
  
  ID= LOR.getSecondHit().getSignalA().getRecoSignal().getPM().getID();
  TOT = LOR.getSecondHit().getSignalA().getPhe();
  Q = LOR.getSecondHit().getSignalA().getRecoSignal().getCharge();
  point = std::make_pair(Q, TOT);
  fValuesPerPMT[ ID ].push_back(point);
  
  
  ID= LOR.getSecondHit().getSignalB().getRecoSignal().getPM().getID();
  TOT = LOR.getSecondHit().getSignalB().getPhe();
  Q = LOR.getSecondHit().getSignalB().getRecoSignal().getCharge();
  point = std::make_pair(Q, TOT);
  fValuesPerPMT[ ID ].push_back(point);
  
    
  fEvent++;
}

void TOTVSQ::end()
{
  
  TGraph* graph;
  
  const JPetParamBank& bank = getParamBank();
  
  std::ofstream file;
  
  
  for(int i = 0; i < bank.getPMsSize(); i++ )
  {
    std::vector<double> TOT, Q;
    std::stringstream buf;
    buf << bank.getPM(i).getID();
    std::string fileName = "TOTs"+buf.str()+".txt";
    file.open(fileName.c_str());
      for(unsigned int j = 0; j < fValuesPerPMT[ bank.getPM(i).getID() ].size(); j++)
      {
	Q.push_back( fValuesPerPMT[ bank.getPM(i).getID() ][j].first );
	TOT.push_back( fValuesPerPMT[ bank.getPM(i).getID() ][j].second );
	file << fValuesPerPMT[ bank.getPM(i).getID() ][j].second << std::endl;
      }
    graph = new TGraph(TOT.size(),&Q[0], &TOT[0]);
    graph->GetXaxis()->SetTitle("Charge [pC]");
    graph->GetYaxis()->SetTitle("TOT [ps]");
    graph->GetXaxis()->SetRangeUser(0,200);
    graph->GetYaxis()->SetRangeUser(0,12000);
    std::stringstream title;
    title << bank.getPM(i).getID() << "Thr" << fThreshold<< fHeader->getSourcePosition() <<  ".png";
    graph->SetTitle(title.str().c_str());
    TCanvas* c = new TCanvas();
    graph->Draw("AP");
    
    c->SaveAs( (title.str()).c_str() );
    
    file.close();
    
    delete c;
    delete graph;
  }
}
