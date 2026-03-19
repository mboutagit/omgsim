//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
#include "KM3RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"


#ifdef G4UI_USE_GAG
#include "G4UIGAG.hh"
#endif
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"
#ifdef G4UI_USE_XM
#include "G4UIXm.hh"
#endif

#ifdef G4UI_USE_Win32
#include "G4UIWin32.hh"
#endif

// #ifdef G4UI_USE_Qt
// #include "G4UIQt.hh" // by Bouta
// #endif

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif
//#include "G4RunManager.hh"


#include "KM3PhysicsList.hh"
#include "KM3EventAction.hh"
#include "KM3RunAction.hh"
#include "KM3PrimaryGeneratorAction.hh"
#include <KM3ExperimentTypeMessenger.hh>
#include "K40AnalysisManager.hh"
#include "KM3SteppingAction.hh"
#include "Randomize.hh"
#include <sys/time.h>
#include <unistd.h>

#include "BAnalysisManager.hh"


#include <random>
#include <iostream>

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

using namespace std;
using namespace km3net;

int main(int argc_,char** argv_)
{
  G4String srf="default.root";
  G4String evtFileName="default.evt";
  string MacFileName;




  bool debug = false;
  bool session = false;
  bool keepall = false;
  bool analyse_all = false;
  bool evt_output = false;
  int runNumber=-1;
  int iarg = 1;
  while (iarg < argc_)
    {
      string token = argv_[iarg];

      if (token[0] == '-')
        {
          string option = token;
          if ((option == "-d") || (option == "--debug"))
            {
              debug = true;
            }
          else if ((option == "-s") || (option == "--session"))
            {
              session = true;
            }
          else if ((option == "-ka") || (option == "--keep-all"))
            {
              keepall = true;
            }
          else if ((option == "-fa") || (option == "--full-analysis"))
            {
              analyse_all=true;
            }
          else if (option == "-evt")
            {
              evt_output=true;
            }
          else if ( (option == "-of") && (iarg+1 <= argc_) )
            {
              srf=argv_[iarg+1];
                iarg++;;

              //std::cout << "\r\033[20C" << "token: " << token << " ---- value: " << argv_[iarg+1] << std::endl;
            }
            else if ( (option == "-evtf") && (iarg+1 <= argc_) )
            {
              evtFileName=argv_[iarg+1];
              //std::cout << "\r\033[20C" << "token: " << token << " ---- value: " << argv_[iarg+1] << std::endl;
              iarg++;;
            }
            else if ( (option == "-r" || option == "-run") && (iarg+1 <= argc_) )
            {

              try {
                  runNumber = std::stoi(argv_[iarg+1]);
                  // success
              } catch (const std::invalid_argument&) {
                  // not a number
                  std::cout << "\r\033[10C" << "Error: run number must be an integer." << std::endl;
                  exit(-1);
              } catch (const std::out_of_range&) {
                  // too large/small for int
                  std::cout << "\r\033[10C" << "Error: Check the run number." << std::endl;
                  exit(-1);
              }

              iarg++;
              //std::cout << "\r\033[20C" << "token: " << token << " ---- value: " << argv_[iarg+1] << std::endl;
            }
           
           

          /* Here you may add more switches...
           * else if (...)
           *  {
           *    ...
           *  }
           */
          else
            {
              clog << "warning: ignoring option " << option << " which does not have a meaning!" << endl;
            }
        }
      else
        {
          string argument = token;
          /* Here you may add more argument handlers... */
          if (!MacFileName.size ())
            {
              MacFileName = token;
            }
          else if (srf == "default.root")
            {
              srf=token;
            }
          else
            {
              clog << "warning: ignoring argument '" << argument << "'!" << endl;
            }
        }
      iarg++;
    }

  try {

    // Creation of the analysis manager as soon as possible to be sure to get the daugther
    //KM3AnalysisManager* analysis = K40AnalysisManager::getIt();
    KM3AnalysisManager::fKeepAll=keepall;
    KM3AnalysisManager::fFastAnalysisOnly=!analyse_all;
    KM3AnalysisManager::fEvtOutput=evt_output;
    KM3AnalysisManager::fVerbose=(short)debug;
    KM3AnalysisManager::fRootFileName = srf;

    lactel::BAnalysisManager::getIt()->evtFileName = evtFileName;
    lactel::BAnalysisManager::getIt()->fRun.runNb=runNumber;

    // random engine
    //CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
    //CLHEP::HepRandom::setTheEngine(new CLHEP::Hurd160Engine);
    CLHEP::HepRandom::setTheEngine(new CLHEP::DualRand);
    //CLHEP::HepRandom::restoreEngineStatus();
    //G4int ttime = time(0);
    //G4cout << "ttime=" << ttime << G4endl;
    timeval tim;
    gettimeofday(&tim, NULL);
    long ttime = tim.tv_sec xor tim.tv_usec;
    G4cout << "ttime=" << ttime << G4endl;

    G4int pid = getpid();
    G4cout << "pid=" << pid << G4endl;

    long Seed = ttime xor (pid << 8);
    // automatic (time-based) random seeds and filenames for each run
    G4cout << "******************" << G4endl;
    G4cout << "*** AUTOSEED ON ***" << G4endl;
    G4cout << "*******************" << G4endl;
    long seeds[2];
    seeds[0] =  Seed;
    seeds[1] =  (long)(Seed*G4UniformRand());
    G4cout << "seed1: " << seeds[0] << "; seed2: " << seeds[1] << G4endl;
  

    CLHEP::HepRandom::getTheEngine()->setSeeds(seeds,-1);
    CLHEP::HepRandom::showEngineStatus();

    // G4RunManager* runManager = G4RunManager::GetRunManager();
    // if(runNumber != -1)
    //   runManager->SetRunID(runNumber);
    KM3ExperimentTypeMessenger* ketm = KM3ExperimentTypeMessenger::GetIt ();
  
    //G4UIsession* session=0;
    // G4UIsession* win32session=0;
    // G4UIsession* sessionQt=0;
     G4VisManager* visManager=NULL;
    if (session)
      {
        //session = new G4UIQt(0, argv_); // by Bouta it was: new G4UIXm(0,argv_);//new G4UIterminal();
        //session = new G4UIXm(0,argv_);//new G4UIterminal()
        //sessionQt = new G4UIQt(0, argv_);
        //win32session = new G4UIWin32(0, argv_);
        // visualization manager

        visManager = new G4VisExecutive;
        visManager->SetVerboseLevel(0); 
        visManager->Initialize();
      //  visManager->SessionStart();
      }


    // Initialize G4 kernel
    // do this at run time so the geometry/physics can be changed
    //  runManager->Initialize();

    // get the pointer to the User Interface manager
    G4UImanager* UI = G4UImanager::GetUIpointer();
    G4String command = "/control/execute ";

  ///////////////////////////////////////////////////////////////////////////////////

    G4UIExecutive* ui = 0;

    if ( ! session ) { 
      // batch mode
      //G4String fileName = argv_[1];
      cout << "\r\033[20C" << "Without session" << endl;
      UI->ApplyCommand(command+MacFileName);
      //UI->ApplyCommand("run/beanOn ");

    }
    else { 
      // interactive mode
      cout << "\r\033[20C" << "With session -- " << endl;
      ui = new G4UIExecutive(argc_, argv_);
      UI->ApplyCommand(command+MacFileName);
      ui->SessionStart();
      delete ui;
    }
/////////////------------------------/////////////////////////////////

    // if (session)   // Define UI session for interactive mode.
    //   {
    //     cout << "Session OK! ..." << endl;
    //     UI->ApplyCommand(command+MacFileName);
    //     cout << "Starting session " << endl;
    //      session->SessionStart();
    //     // sessionQt->SessionStart();
    //     cout << "toward deleting session " << endl;
    //     delete session;
    //   }
    // else           // Batch mode
    //   {
    //     cout << "Session NOT OK! ..." << endl;
    //     UI->ApplyCommand(command+MacFileName);
    //     //session->SessionStart ();
    //   }

 /////////////////////////////////////////////////////////////////////////////////////////////////////
    //analysis->Write ();
    //CLHEP::HepRandom::showEngineStatus();
    //CLHEP::HepRandom::getTheTableSeeds(table, 0);
    //for (int i=0; i< sizeof(table); i++)
    //  G4cout << table[i] << G4endl;
    //CLHEP::HepRandom::saveEngineStatus();
    // job termination
    if (visManager)
      {
        cout << "Deleting visManager !! " << endl;
        delete visManager;
      }
      if(ui){
        delete ui;
      }


    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(-17*2.54/2, 17*2.54/2);

    //delete ger;
    delete ketm;
  }
  catch (const char* err)
    {
      cerr << "Exception occured: " << err << endl;
      return 1;
    }

  return 0;

}
