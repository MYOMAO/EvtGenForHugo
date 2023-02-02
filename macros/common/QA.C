#ifndef MACRO_QA_C
#define MACRO_QA_C

#include <fun4all/Fun4AllServer.h>
#include <qa_modules/QAG4SimulationCalorimeterSum.h>
#include "/sphenix/user/zshi/EvtGenPullRequest/install/include/qa_modules/QAG4Decayer.h"

#include <qa_modules/QAHistManagerDef.h>
#include <qa_modules/QAG4SimulationTruthDecay.h>


R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libqa_modules.so)

namespace Enable
{
  // if you want this to run by default, initialize this to true
  // Otherwise you have to use Enable::USER = true; in your macro
  bool QA = false;
  int QA_VERBOSITY = 0;
}  // namespace Enable

namespace QA
{
  //  int myparam = 0;
}  // namespace QA

void QA_Official(){
  Fun4AllServer* se = Fun4AllServer::instance();	
  std::cout << "Test Official QA Codes" << std::endl;
  QAG4SimulationTruthDecay *myTester = new QAG4SimulationTruthDecay();
  myTester->setMotherPDG(421);
  myTester->setDFNodeName("myFinder");
  myTester->setOutputName("CheckOfficial.root");
  se->registerSubsystem(myTester);
  return;
}

// QA moduel with GEANT Decayer
void QA_G4Decayer()
{
  Fun4AllServer* se = Fun4AllServer::instance();
  QAG4Decayer* decayer_qa = new QAG4Decayer();
  se->registerSubsystem(decayer_qa);
  return;
}

// QA moduel combining tracking and calorimeters
void QA_G4CaloTracking()
{
  Fun4AllServer* se = Fun4AllServer::instance();
  QAG4SimulationCalorimeterSum* calo_qa = new QAG4SimulationCalorimeterSum();
  calo_qa->Verbosity(Enable::QA_VERBOSITY);
  se->registerSubsystem(calo_qa);
  return;
}

// run this after se->run() to output QA histogram file for all QA modules
void QA_Output(const std::string& qaOutputFileName)
{
  Fun4AllServer* se = Fun4AllServer::instance();
  QAHistManagerDef::saveQARootFile(qaOutputFileName);
  return;
}

#endif
