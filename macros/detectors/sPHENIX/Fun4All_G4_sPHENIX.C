#ifndef MACRO_FUN4ALLG4SPHENIX_C
#define MACRO_FUN4ALLG4SPHENIX_C

#include <GlobalVariables.C>

#include <DisplayOn.C>
#include <G4Setup_sPHENIX.C>

#include <G4_Bbc.C>
#include <G4_CaloTrigger.C>
#include <G4_Centrality.C>
#include <G4_DSTReader.C>
#include <G4_Global.C>
#include <G4_HIJetReco.C>
#include <G4_Input.C>
#include <G4_Jets.C>
#include <G4_KFParticle.C>
#include <G4_ParticleFlow.C>
#include <G4_Production.C>
#include <G4_TopoClusterReco.C>
#include <G4_Tracking.C>
#include <G4_User.C>

#include <QA.C>

#include <ffamodules/FlagHandler.h>
#include <ffamodules/HeadReco.h>
#include <ffamodules/SyncReco.h>
#include <ffamodules/XploadInterface.h>

#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllServer.h>

#include <phool/PHRandomSeed.h>
#include <phool/recoConsts.h>


#include <decayfinder/DecayFinder.h>
#include <qa_modules/QAG4SimulationTruthDecay.h>

#include <phpythia8/PHPy8ParticleTrigger.h>

#include <phpythia8/PHPy8JetTrigger.h>  //Trigger on jets


R__LOAD_LIBRARY(libqa_modules.so)
R__LOAD_LIBRARY(libdecayfinder.so)

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libffamodules.so)

// For HepMC Hijing
// try inputFile = /sphenix/sim/sim01/sphnxpro/sHijing_HepMC/sHijing_0-12fm.dat

int Fun4All_G4_sPHENIX(
    const int nEvents = 1,
    const string &inputFile = "https://www.phenix.bnl.gov/WWW/publish/phnxbld/sPHENIX/files/sPHENIX_G4Hits_sHijing_9-11fm_00000_00010.root",
    const string &outputFile = "G4sPHENIX.root",
    const string &embed_input_file = "https://www.phenix.bnl.gov/WWW/publish/phnxbld/sPHENIX/files/sPHENIX_G4Hits_sHijing_9-11fm_00000_00010.root",
    const int skip = 0,
    const string &outdir = ".")
{
  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(0);

  //Opt to print all random seed used for debugging reproducibility. Comment out to reduce stdout prints.
  PHRandomSeed::Verbosity(1);

  // just if we set some flags somewhere in this macro
  recoConsts *rc = recoConsts::instance();
  // By default every random number generator uses
  // PHRandomSeed() which reads /dev/urandom to get its seed
  // if the RANDOMSEED flag is set its value is taken as seed
  // You can either set this to a random value using PHRandomSeed()
  // which will make all seeds identical (not sure what the point of
  // this would be:
  //  rc->set_IntFlag("RANDOMSEED",PHRandomSeed());
  // or set it to a fixed value so you can debug your code
    rc->set_IntFlag("RANDOMSEED", 12345);

std::cout <<"SEED FIXED" << std::endl;

  //===============
  // Input options
  //===============
  // verbosity setting (applies to all input managers)
  Input::VERBOSITY = 0;
  // First enable the input generators
  // Either:
  // read previously generated g4-hits files, in this case it opens a DST and skips
  // the simulations step completely. The G4Setup macro is only loaded to get information
  // about the number of layers used for the cell reco code
  //  Input::READHITS = true;
  INPUTREADHITS::filename[0] = inputFile;
  // if you use a filelist
  // INPUTREADHITS::listfile[0] = inputFile;
  // Or:
  // Use particle generator
  // And
  // Further choose to embed newly simulated events to a previous simulation. Not compatible with `readhits = true`
  // In case embedding into a production output, please double check your G4Setup_sPHENIX.C and G4_*.C consistent with those in the production macro folder
  // E.g. /sphenix/sim//sim01/production/2016-07-21/single_particle/spacal2d/
  //  Input::EMBED = true;
  INPUTEMBED::filename[0] = embed_input_file;
  // if you use a filelist
  //INPUTEMBED::listfile[0] = embed_input_file;

  Input::SIMPLE = true;

  // Input::SIMPLE_NUMBER = 2; // if you need 2 of them
  // Input::SIMPLE_VERBOSITY = 1;

 //  Input::PYTHIA6 = true;

 // Input::PYTHIA8 = true;

  //  Input::GUN = true;
  //  Input::GUN_NUMBER = 3; // if you need 3 of them
  // Input::GUN_VERBOSITY = 1;

  //D0 generator
  //Input::DZERO = false;
  //Input::DZERO_VERBOSITY = 0;
  //Lambda_c generator //Not ready yet
  //Input::LAMBDAC = false;
  //Input::LAMBDAC_VERBOSITY = 0;
  // Upsilon generator
  //Input::UPSILON = true;
  //Input::UPSILON_NUMBER = 3; // if you need 3 of them
  //Input::UPSILON_VERBOSITY = 0;

  //  Input::HEPMC = true;
  INPUTHEPMC::filename = inputFile;

  // Event pile up simulation with collision rate in Hz MB collisions.
  //Input::PILEUPRATE = 100e3;

  //-----------------
  // Initialize the selected Input/Event generation
  //-----------------
  // This creates the input generator(s)
  //string pythia8_config_file = "phpythia8_Bs2Dspi.cfg";//string(getenv("CALIBRATIONROOT")) + "/Generators/HeavyFlavor_TG/phpythia8_b2DX_MDC2.cfg";
//  string pythia8_config_file = "phpythia8_D02Kpi.cfg";//string(getenv("CALIBRATIONROOT")) + "/Generators/HeavyFlavor_TG/phpythia8_b2DX_MDC2.cfg";
  string pythia8_config_file = "phpythia8_Check.cfg";

  PYTHIA8::config_file = pythia8_config_file;
  InputInit();

/*
 if (Input::PYTHIA8)
  {
	std::cout << "Using PYTHIA b jet" << std::endl;

    PHPy8ParticleTrigger * p8_hf_signal_trigger = new PHPy8ParticleTrigger();
    p8_hf_signal_trigger->AddParticles(5);
    p8_hf_signal_trigger->AddParticles(-5);
    p8_hf_signal_trigger->SetYHighLow(1.5, -1.5); // sample a rapidity range higher than the sPHENIX tracking pseudorapidity
    p8_hf_signal_trigger->SetStableParticleOnly(false); // process unstable particles that include quarks
//    p8_hf_signal_trigger->PrintConfig();
    p8_hf_signal_trigger->Verbosity(0);
    //
    INPUTGENERATOR::Pythia8->register_trigger(p8_hf_signal_trigger);

    PHPy8JetTrigger *p8_jet_signal_trigger = new PHPy8JetTrigger();
    p8_jet_signal_trigger->SetEtaHighLow(1.1, -1.1);
    p8_jet_signal_trigger->SetMinJetPt(10.);
  //  p8_jet_signal_trigger->PrintConfig();
    INPUTGENERATOR::Pythia8->register_trigger(p8_jet_signal_trigger);

    INPUTGENERATOR::Pythia8->set_trigger_AND();
    Input::ApplysPHENIXBeamParameter(INPUTGENERATOR::Pythia8);
  
   }
*/
  //--------------
  // Set generator specific options
  //--------------
  // can only be set after InputInit() is called

  // Simple Input generator:
  // if you run more than one of these Input::SIMPLE_NUMBER > 1
  // add the settings for other with [1], next with [2]...


  if (Input::SIMPLE)
  {
 //    INPUTGENERATOR::SimpleEventGenerator[0]->add_particles(4132, 1);
  // INPUTGENERATOR::SimpleEventGenerator[0]->add_particles(411,1);
  //   INPUTGENERATOR::SimpleEventGenerator[0]->add_particles(553,1);
    INPUTGENERATOR::SimpleEventGenerator[0]->add_particles(443,1);
   // INPUTGENERATOR::SimpleEventGenerator[0]->add_particles(531,1); 
//  INPUTGENERATOR::SimpleEventGenerator[0]->add_particles(4122,1);
//     const int NHFQA = 13;
//   int QAVtxPDGID[NHFQA] = {411,421,431,4122,511,521,531,553,443,100443,200443,100553,200553};
//    INPUTGENERATOR::SimpleEventGenerator[0]->add_particles(113,1);

/*
	const int NHFQA = 9;
int QAVtxPDGID[NHFQA] = {411,421,431,4122,511,521,531,443,553};
	 for(int q = 0; q < NHFQA; q++){
		 INPUTGENERATOR::SimpleEventGenerator[0]->add_particles(QAVtxPDGID[q],1);
	 }
*/
	 // INPUTGENERATOR::SimpleEventGenerator[0]->add_particles(12,1);  
   if (Input::HEPMC || Input::EMBED)
    {
      INPUTGENERATOR::SimpleEventGenerator[0]->set_reuse_existing_vertex(true);
      INPUTGENERATOR::SimpleEventGenerator[0]->set_existing_vertex_offset_vector(0.0, 0.0, 0.0);
    }
    else
    {
      INPUTGENERATOR::SimpleEventGenerator[0]->set_vertex_distribution_function(PHG4SimpleEventGenerator::Gaus,
                                                                                PHG4SimpleEventGenerator::Gaus,
                                                                                PHG4SimpleEventGenerator::Gaus);
      INPUTGENERATOR::SimpleEventGenerator[0]->set_vertex_distribution_mean(0., 0, 0);
      INPUTGENERATOR::SimpleEventGenerator[0]->set_vertex_distribution_width(0.0, 0.0, 0);
    }
    INPUTGENERATOR::SimpleEventGenerator[0]->set_eta_range(0, 0);
    INPUTGENERATOR::SimpleEventGenerator[0]->set_phi_range(-M_PI, M_PI);
    INPUTGENERATOR::SimpleEventGenerator[0]->set_p_range(1, 1);
  //  INPUTGENERATOR::SimpleEventGenerator[0]->set_pt_range(0.00000, 0.0000000001);

  }
  // Upsilons
  // if you run more than one of these Input::UPSILON_NUMBER > 1
  // add the settings for other with [1], next with [2]...
  if (Input::UPSILON)
  {
    INPUTGENERATOR::VectorMesonGenerator[0]->add_decay_particles("e", 0);
    INPUTGENERATOR::VectorMesonGenerator[0]->set_rapidity_range(-1, 1);
    INPUTGENERATOR::VectorMesonGenerator[0]->set_pt_range(0., 10.);
    // Y species - select only one, last one wins
    INPUTGENERATOR::VectorMesonGenerator[0]->set_upsilon_1s();
    if (Input::HEPMC || Input::EMBED)
    {
      INPUTGENERATOR::VectorMesonGenerator[0]->set_reuse_existing_vertex(true);
      INPUTGENERATOR::VectorMesonGenerator[0]->set_existing_vertex_offset_vector(0.0, 0.0, 0.0);
    }
  }
  // particle gun
  // if you run more than one of these Input::GUN_NUMBER > 1
  // add the settings for other with [1], next with [2]...
  if (Input::GUN)
  {
    INPUTGENERATOR::Gun[0]->AddParticle("pi-", 0, 1, 0);
    INPUTGENERATOR::Gun[0]->set_vtx(0, 0, 0);
  }

  // pythia6
  if (Input::PYTHIA6)
  {
    //! apply sPHENIX nominal beam parameter with 2mrad crossing as defined in sPH-TRG-2020-001
    Input::ApplysPHENIXBeamParameter(INPUTGENERATOR::Pythia6);
  }
  // pythia8
  if (Input::PYTHIA8)
  {
    //! apply sPHENIX nominal beam parameter with 2mrad crossing as defined in sPH-TRG-2020-001
    Input::ApplysPHENIXBeamParameter(INPUTGENERATOR::Pythia8);
  }

  //--------------
  // Set Input Manager specific options
  //--------------
  // can only be set after InputInit() is called

  if (Input::HEPMC)
  {
    //! apply sPHENIX nominal beam parameter with 2mrad crossing as defined in sPH-TRG-2020-001
    Input::ApplysPHENIXBeamParameter(INPUTMANAGER::HepMCInputManager);

    // optional overriding beam parameters
    //INPUTMANAGER::HepMCInputManager->set_vertex_distribution_width(100e-4, 100e-4, 8, 0);  //optional collision smear in space, time
    //    INPUTMANAGER::HepMCInputManager->set_vertex_distribution_mean(0,0,0,0);//optional collision central position shift in space, time
    // //optional choice of vertex distribution function in space, time
    //INPUTMANAGER::HepMCInputManager->set_vertex_distribution_function(PHHepMCGenHelper::Gaus, PHHepMCGenHelper::Gaus, PHHepMCGenHelper::Gaus, PHHepMCGenHelper::Gaus);
    //! embedding ID for the event
    //! positive ID is the embedded event of interest, e.g. jetty event from pythia
    //! negative IDs are backgrounds, .e.g out of time pile up collisions
    //! Usually, ID = 0 means the primary Au+Au collision background
    //INPUTMANAGER::HepMCInputManager->set_embedding_id(Input::EmbedID);
    if (Input::PILEUPRATE > 0)
    {
      // Copy vertex settings from foreground hepmc input
      INPUTMANAGER::HepMCPileupInputManager->CopyHelperSettings(INPUTMANAGER::HepMCInputManager);
      // and then modify the ones you want to be different
      // INPUTMANAGER::HepMCPileupInputManager->set_vertex_distribution_width(100e-4,100e-4,8,0);
    }
  }
  if (Input::PILEUPRATE > 0)
  {
    //! apply sPHENIX nominal beam parameter with 2mrad crossing as defined in sPH-TRG-2020-001
    Input::ApplysPHENIXBeamParameter(INPUTMANAGER::HepMCPileupInputManager);
  }
  // register all input generators with Fun4All
  InputRegister();

  if (! Input::READHITS)
  {
    rc->set_IntFlag("RUNNUMBER",1);

    SyncReco *sync = new SyncReco();
    se->registerSubsystem(sync);

    HeadReco *head = new HeadReco();
    se->registerSubsystem(head);
  }
// Flag Handler is always needed to read flags from input (if used)
// and update our rc flags with them. At the end it saves all flags
// again on the DST in the Flags node under the RUN node
  FlagHandler *flag = new FlagHandler();
  se->registerSubsystem(flag);

  // set up production relatedstuff
  //   Enable::PRODUCTION = true;

  //======================
  // Write the DST
  //======================

  //Enable::DSTOUT = true;
  Enable::DSTOUT_COMPRESS = false;
  DstOut::OutputDir = outdir;
  DstOut::OutputFile = outputFile;

  //Option to convert DST to human command readable TTree for quick poke around the outputs
  //  Enable::DSTREADER = true;

  // turn the display on (default off)
   //Enable::DISPLAY = true;

  //======================
  // What to run
  //======================

  // QA, main switch
  Enable::QA = true;

  // Global options (enabled for all enables subsystems - if implemented)
  //  Enable::ABSORBER = true;
  //  Enable::OVERLAPCHECK = true;
  //  Enable::VERBOSITY = 1;

  // Enable::BBC = true;
  // Enable::BBC_SUPPORT = true; // save hist in bbc support structure
  Enable::BBCFAKE = false;  // Smeared vtx and t0, use if you don't want real BBC in simulation

  Enable::PIPE = false;
  Enable::PIPE_ABSORBER = false;

  // central tracking
  Enable::MVTX = false;
  Enable::MVTX_CELL = Enable::MVTX && true;
  Enable::MVTX_CLUSTER = Enable::MVTX_CELL && true;
  Enable::MVTX_QA = Enable::MVTX_CLUSTER && Enable::QA && true;

  Enable::INTT = false;
//  Enable::INTT_ABSORBER = true; // enables layerwise support structure readout
//  Enable::INTT_SUPPORT = true; // enable global support structure readout
  Enable::INTT_CELL = Enable::INTT && true;
  Enable::INTT_CLUSTER = Enable::INTT_CELL && true;
  Enable::INTT_QA = Enable::INTT_CLUSTER && Enable::QA && true;

  Enable::TPC = false;
  Enable::TPC_ABSORBER = false;
  Enable::TPC_CELL = Enable::TPC && true;
  Enable::TPC_CLUSTER = Enable::TPC_CELL && true;
  Enable::TPC_QA = Enable::TPC_CLUSTER && Enable::QA && true;

  Enable::MICROMEGAS = false;

  Enable::MICROMEGAS_CELL = Enable::MICROMEGAS && false;
  Enable::MICROMEGAS_CLUSTER = Enable::MICROMEGAS_CELL && true;
  Enable::MICROMEGAS_QA = Enable::MICROMEGAS_CLUSTER && Enable::QA && true;

  Enable::TRACKING_TRACK = (Enable::MICROMEGAS_CLUSTER && Enable::TPC_CLUSTER && Enable::INTT_CLUSTER && Enable::MVTX_CLUSTER) && true;
  Enable::TRACKING_EVAL = Enable::TRACKING_TRACK && true;
  Enable::TRACKING_QA = Enable::TRACKING_TRACK && Enable::QA && true;

  //  cemc electronics + thin layer of W-epoxy to get albedo from cemc
  //  into the tracking, cannot run together with CEMC
  //  Enable::CEMCALBEDO = true;

  Enable::CEMC = false;
  Enable::CEMC_ABSORBER = false;
  Enable::CEMC_CELL = Enable::CEMC && true;
  Enable::CEMC_TOWER = Enable::CEMC_CELL && true;
  Enable::CEMC_CLUSTER = Enable::CEMC_TOWER && true;
  Enable::CEMC_EVAL = Enable::CEMC_CLUSTER && true;
  Enable::CEMC_QA = Enable::CEMC_CLUSTER && Enable::QA && true;

  Enable::HCALIN = false;
  Enable::HCALIN_ABSORBER = true;
  Enable::HCALIN_CELL = Enable::HCALIN && true;
  Enable::HCALIN_TOWER = Enable::HCALIN_CELL && true;
  Enable::HCALIN_CLUSTER = Enable::HCALIN_TOWER && true;
  Enable::HCALIN_EVAL = Enable::HCALIN_CLUSTER && true;
  Enable::HCALIN_QA = Enable::HCALIN_CLUSTER && Enable::QA && true;

  Enable::MAGNET = false;
  Enable::MAGNET_ABSORBER = false;

  Enable::HCALOUT = false;
  Enable::HCALOUT_ABSORBER = false;
  Enable::HCALOUT_CELL = Enable::HCALOUT && true;
  Enable::HCALOUT_TOWER = Enable::HCALOUT_CELL && true;
  Enable::HCALOUT_CLUSTER = Enable::HCALOUT_TOWER && true;
  Enable::HCALOUT_EVAL = Enable::HCALOUT_CLUSTER && true;
  Enable::HCALOUT_QA = Enable::HCALOUT_CLUSTER && Enable::QA && true;

  Enable::EPD = false;

  Enable::BEAMLINE = false;
//  Enable::BEAMLINE_ABSORBER = true;  // makes the beam line magnets sensitive volumes
//  Enable::BEAMLINE_BLACKHOLE = true; // turns the beamline magnets into black holes
  Enable::ZDC = false;
//  Enable::ZDC_ABSORBER = true;
//  Enable::ZDC_SUPPORT = true;
  Enable::ZDC_TOWER = Enable::ZDC && true;
  Enable::ZDC_EVAL = Enable::ZDC_TOWER && true;

  //! forward flux return plug door. Out of acceptance and off by default.
  //Enable::PLUGDOOR = true;
  Enable::PLUGDOOR_ABSORBER = false;

  Enable::GLOBAL_RECO = (Enable::BBCFAKE || Enable::TRACKING_TRACK) && true;
  //Enable::GLOBAL_FASTSIM = true;

  //Enable::KFPARTICLE = true;
  //Enable::KFPARTICLE_VERBOSITY = 1;
  //Enable::KFPARTICLE_TRUTH_MATCH = true;
  //Enable::KFPARTICLE_SAVE_NTUPLE = true;

  Enable::CALOTRIGGER = Enable::CEMC_TOWER && Enable::HCALIN_TOWER && Enable::HCALOUT_TOWER && false;

  Enable::JETS = (Enable::GLOBAL_RECO || Enable::GLOBAL_FASTSIM) && false;
  Enable::JETS_EVAL = Enable::JETS && true;
  Enable::JETS_QA = Enable::JETS && Enable::QA && true;

  // HI Jet Reco for p+Au / Au+Au collisions (default is false for
  // single particle / p+p-only simulations, or for p+Au / Au+Au
  // simulations which don't particularly care about jets)
  Enable::HIJETS = Enable::JETS && Enable::CEMC_TOWER && Enable::HCALIN_TOWER && Enable::HCALOUT_TOWER && false;

  // 3-D topoCluster reconstruction, potentially in all calorimeter layers
  Enable::TOPOCLUSTER = Enable::CEMC_TOWER && Enable::HCALIN_TOWER && Enable::HCALOUT_TOWER && false;
  // particle flow jet reconstruction - needs topoClusters!
  Enable::PARTICLEFLOW = Enable::TOPOCLUSTER && true;
  // centrality reconstruction
  Enable::CENTRALITY = false;

  // new settings using Enable namespace in GlobalVariables.C
  Enable::BLACKHOLE = true;
  Enable::BLACKHOLE_SAVEHITS = true; // turn off saving of bh hits
  Enable::BLACKHOLE_FORWARD_SAVEHITS = true; // disable forward/backward hits
  BlackHoleGeometry::visible = false;

  // run user provided code (from local G4_User.C)
  //Enable::USER = true;
/*
  PHG4Reco *g4Reco = new PHG4Reco();
  g4Reco->set_field(0);  // field off
  g4Reco->SetWorldMaterial("G4_Galactic");
  g4Reco->SetWorldSizeX(10.);
  g4Reco->SetWorldSizeZ(120.);
  g4Reco->SetWorldSizeY(10.);
  g4Reco->save_DST_geometry(false);  // no geometry -> crashes when trying to save it
//  g4Reco->set_decayer_active(false);

  PHG4CylinderSubsystem *cyl = new PHG4CylinderSubsystem("BH", 0);
  cyl->set_double_param("radius", 1.);
  if(nEvents >0)
    {
      cyl->set_string_param("material", "G4_Galactic");
      cyl->BlackHole();
    }
  else cyl->set_string_param("material", "G4_POLYSTYRENE");
  cyl->set_double_param("thickness", 0.01);
  cyl->set_double_param("length", 100);
  cyl->SetActive();
  g4Reco->registerSubsystem(cyl);

  bool addEndcaps = true;

  if(addEndcaps)
    {
      PHG4BlockSubsystem *box = new PHG4BlockSubsystem("Rightside1");
      box->set_double_param("size_x",10);
      box->set_double_param("size_y",10.);
      box->set_double_param("size_z",1);
      box->set_double_param("place_x",0.);
      box->set_double_param("place_y",0.);
      box->set_double_param("place_z",50.);
      box->set_double_param("rot_x",180.);
      box->set_double_param("rot_y",180.);
      box->set_double_param("rot_z",180.);
      if(nEvents >0)
	{
	  box ->set_string_param("material", "G4_Galactic"); 
	  box -> BlackHole();
	}
      else box->set_string_param("material","G4_POLYSTYRENE"); // material of box
      box->SetActive(); // it is an active volume - save G4Hits
      g4Reco->registerSubsystem(box);

      PHG4BlockSubsystem *box2 = new PHG4BlockSubsystem("Leftside1");
      box2->set_double_param("size_x",10);
      box2->set_double_param("size_y",10.);
      box2->set_double_param("size_z",1);
      box2->set_double_param("place_x",0.);
      box2->set_double_param("place_y",0.);
      box2->set_double_param("place_z",-50.);
      box2->set_double_param("rot_x",180.);
      box2->set_double_param("rot_y",180.);
      box2->set_double_param("rot_z",180.);
      if(nEvents >0)
	{
	  box2 ->set_string_param("material", "G4_Galactic"); 
	  box2 -> BlackHole();
	}
      else box2->set_string_param("material","G4_POLYSTYRENE"); // material of box
      box2->SetActive(); // it is an active volume - save G4Hits
      g4Reco->registerSubsystem(box2);
    }
  PHG4TruthSubsystem *truth = new PHG4TruthSubsystem();
  g4Reco->registerSubsystem(truth);

 
  se->registerSubsystem(g4Reco);
 
*/


  //===============
  // conditions DB flags
  //===============
  //Enable::XPLOAD = true;
  // tag
  rc->set_StringFlag("XPLOAD_TAG",XPLOAD::tag);
  // database config
  rc->set_StringFlag("XPLOAD_CONFIG",XPLOAD::config);
  // 64 bit timestamp
  rc->set_uint64Flag("TIMESTAMP",XPLOAD::timestamp);

  //---------------
  // World Settings
  //---------------
  //  G4WORLD::PhysicsList = "FTFP_BERT"; //FTFP_BERT_HP best for calo
  //  G4WORLD::WorldMaterial = "G4_AIR"; // set to G4_GALACTIC for material scans

  //---------------
  // Magnet Settings
  //---------------

  //  G4MAGNET::magfield =  string(getenv("CALIBRATIONROOT"))+ string("/Field/Map/sphenix3dbigmapxyz.root");  // default map from the calibration database
  //  G4MAGNET::magfield = "1.5"; // alternatively to specify a constant magnetic field, give a float number, which will be translated to solenoidal field in T, if string use as fieldmap name (including path)
//  G4MAGNET::magfield_rescale = 1.;  // make consistent with expected Babar field strength of 1.4T

  //---------------
  // Pythia Decayer
  //---------------
  // list of decay types in
  // $OFFLINE_MAIN/include/g4decayer/EDecayType.hh
  // default is All:
  // G4P6DECAYER::decayType = EDecayType::kAll;

  // Initialize the selected subsystems
  G4Init();

  //---------------------
  // GEANT4 Detector description
  //---------------------
  if (!Input::READHITS)
  {
    G4Setup();
  }

  //------------------
  // Detector Division
  //------------------

  if (Enable::BBC || Enable::BBCFAKE) Bbc_Reco();

  if (Enable::MVTX_CELL) Mvtx_Cells();
  if (Enable::INTT_CELL) Intt_Cells();
  if (Enable::TPC_CELL) TPC_Cells();
  if (Enable::MICROMEGAS_CELL) Micromegas_Cells();

  if (Enable::CEMC_CELL) CEMC_Cells();

  if (Enable::HCALIN_CELL) HCALInner_Cells();

  if (Enable::HCALOUT_CELL) HCALOuter_Cells();

  //-----------------------------
  // CEMC towering and clustering
  //-----------------------------

  if (Enable::CEMC_TOWER) CEMC_Towers();
  if (Enable::CEMC_CLUSTER) CEMC_Clusters();

  //-----------------------------
  // HCAL towering and clustering
  //-----------------------------

  if (Enable::HCALIN_TOWER) HCALInner_Towers();
  if (Enable::HCALIN_CLUSTER) HCALInner_Clusters();

  if (Enable::HCALOUT_TOWER) HCALOuter_Towers();
  if (Enable::HCALOUT_CLUSTER) HCALOuter_Clusters();

  // if enabled, do topoClustering early, upstream of any possible jet reconstruction
  if (Enable::TOPOCLUSTER) TopoClusterReco();

  //--------------
  // SVTX tracking
  //--------------
  if(Enable::TRACKING_TRACK)
    {
      TrackingInit();
    }
  if (Enable::MVTX_CLUSTER) Mvtx_Clustering();
  if (Enable::INTT_CLUSTER) Intt_Clustering();
  if (Enable::TPC_CLUSTER) TPC_Clustering();
  if (Enable::MICROMEGAS_CLUSTER) Micromegas_Clustering();

  if (Enable::TRACKING_TRACK)
  {
    Tracking_Reco();
  }
  //-----------------
  // Global Vertexing
  //-----------------

  if (Enable::GLOBAL_RECO && Enable::GLOBAL_FASTSIM)
  {
    cout << "You can only enable Enable::GLOBAL_RECO or Enable::GLOBAL_FASTSIM, not both" << endl;
    gSystem->Exit(1);
  }
  if (Enable::GLOBAL_RECO)
  {
    Global_Reco();
  }
  else if (Enable::GLOBAL_FASTSIM)
  {
    Global_FastSim();
  }

  //-----------------
  // Centrality Determination
  //-----------------

  if (Enable::CENTRALITY)
  {
      Centrality();
  }

  //-----------------
  // Calo Trigger Simulation
  //-----------------

  if (Enable::CALOTRIGGER)
  {
    CaloTrigger_Sim();
  }

  //---------
  // Jet reco
  //---------

  if (Enable::JETS) Jet_Reco();
  if (Enable::HIJETS) HIJetReco();

  if (Enable::PARTICLEFLOW) ParticleFlow();

  //----------------------
  // Simulation evaluation
  //----------------------
  string outputroot = outputFile;
  string remove_this = ".root";
  size_t pos = outputroot.find(remove_this);
  if (pos != string::npos)
  {
    outputroot.erase(pos, remove_this.length());
  }

  if (Enable::TRACKING_EVAL) Tracking_Eval(outputroot + "_g4svtx_eval.root");

  if (Enable::CEMC_EVAL) CEMC_Eval(outputroot + "_g4cemc_eval.root");

  if (Enable::HCALIN_EVAL) HCALInner_Eval(outputroot + "_g4hcalin_eval.root");

  if (Enable::HCALOUT_EVAL) HCALOuter_Eval(outputroot + "_g4hcalout_eval.root");

  if (Enable::JETS_EVAL) Jet_Eval(outputroot + "_g4jet_eval.root");

  if (Enable::DSTREADER) G4DSTreader(outputroot + "_DSTReader.root");

  if (Enable::USER) UserAnalysisInit();

  //======================
  // Run KFParticle on evt
  //======================
  if (Enable::KFPARTICLE && Input::UPSILON) KFParticle_Upsilon_Reco();
  if (Enable::KFPARTICLE && Input::DZERO) KFParticle_D0_Reco();

  //----------------------
  // Standard QAs
  //----------------------

  if (Enable::CEMC_QA) CEMC_QA();
  if (Enable::HCALIN_QA) HCALInner_QA();
  if (Enable::HCALOUT_QA) HCALOuter_QA();


  if (Enable::MVTX_QA) Mvtx_QA();
  if (Enable::INTT_QA) Intt_QA();
  if (Enable::TPC_QA) TPC_QA();
  if (Enable::MICROMEGAS_QA) Micromegas_QA();
  if (Enable::TRACKING_QA) Tracking_QA();

  if (Enable::TRACKING_QA && Enable::CEMC_QA && Enable::HCALIN_QA && Enable::HCALOUT_QA) QA_G4CaloTracking();

  //--------------
  // Set up Input Managers
  //--------------

  //--------- G4 EvtGen Decayer QA Bro ------------

//  QA_G4Decayer();
  std::cout << "Also Official QA Bro" << std::endl;
   //QA_Official();
  //---------------
  
  InputManagers();

  if (Enable::PRODUCTION)
  {
    Production_CreateOutputDir();
  }

  if (Enable::DSTOUT)
  {
    string FullOutFile = DstOut::OutputDir + "/" + DstOut::OutputFile;
    Fun4AllDstOutputManager *out = new Fun4AllDstOutputManager("DSTOUT", FullOutFile);
    if (Enable::DSTOUT_COMPRESS)
    {
      ShowerCompress();
      DstCompress(out);
    }
    se->registerOutputManager(out);
  }
  //-----------------
  // Event processing
  //-----------------
  if (Enable::DISPLAY)
  {
    DisplayOn();

    gROOT->ProcessLine("Fun4AllServer *se = Fun4AllServer::instance();");
    gROOT->ProcessLine("PHG4Reco *g4 = (PHG4Reco *) se->getSubsysReco(\"PHG4RECO\");");

    cout << "-------------------------------------------------" << endl;
    cout << "You are in event display mode. Run one event with" << endl;
    cout << "se->run(1)" << endl;
    cout << "Run Geant4 command with following examples" << endl;
    gROOT->ProcessLine("displaycmd()");

    return 0;
  }

  // if we use a negative number of events we go back to the command line here
  if (nEvents < 0)
  {
    return 0;
  }
  // if we run the particle generator and use 0 it'll run forever
  // for embedding it runs forever if the repeat flag is set
  if (nEvents == 0 && !Input::HEPMC && !Input::READHITS && INPUTEMBED::REPEAT)
  {
    cout << "using 0 for number of events is a bad idea when using particle generators" << endl;
    cout << "it will run forever, so I just return without running anything" << endl;
    return 0;
  }

/*
  DecayFinder *myFinder = new DecayFinder("myFinder");
  myFinder->Verbosity(0);
//  myFinder->setDecayDescriptor("");
//   myFinder->setDecayDescriptor("[D0 -> K-^- pi^+]cc");
  myFinder->setDecayDescriptor("[B_s0 -> K^- K^+ pi^- pi^+]cc");
  myFinder->saveDST(1);
  myFinder->allowPi0(0);
  myFinder->allowPhotons(0);
  myFinder->triggerOnDecay(1);
  myFinder->setPTmin(0.);
  myFinder->setEtaRange(-10., 10.);
  se->registerSubsystem(myFinder);


  QAG4SimulationTruthDecay *myTester = new QAG4SimulationTruthDecay();
  myTester->setMotherPDG(531);
  myTester->setDFNodeName("myFinder");
  myTester->setOutputName("Camerontoutput.root");
  se->registerSubsystem(myTester);

*/

  QA_G4Decayer();


  se->skip(skip);
  se->run(nEvents);

  //-----
  // QA output
  //-----

  

  if (Enable::QA) QA_Output("PHOTOS_Jpsi_QA.root");

  //-----
  // Exit
  //-----


  se->End();
  std::cout << "All done" << std::endl;
  delete se;
  if (Enable::PRODUCTION)
  {
    Production_MoveOutput();
  }

  gSystem->Exit(0);
  return 0;
}
#endif
