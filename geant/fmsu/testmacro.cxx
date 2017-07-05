int main(int argc, char** argv)
{
//////////////////////////////////////////
// INITIALIZING ALL OUR GEANT CLASSES

  // User Verbose output class
  //
  //t G4VSteppingVerbose* verbosity = new SteppingVerbose;
  //t G4VSteppingVerbose::SetInstance(verbosity);

  // Run manager
  //
  G4RunManager * runManager = new G4RunManager;

  // User Initialization classes (mandatory)
  //
  DetectorConstruction* detector = new DetectorConstruction;
  runManager->SetUserInitialization(detector);
  //
  //Note only those particles that are listed in PhysicsList.cc will be included, attempting to call otherwise will result in fatal error
  G4VUserPhysicsList* physics = new PhysicsList;
  runManager->SetUserInitialization(physics);

  runManager->SetUserAction(new PrimaryGeneratorAction(detector));  //Create particle gun and initilize default values
  runManager->SetUserAction(new RunAction);
  pFile->cd();

  TTree *geotr = new TTree("geotr","geotr");
  EventAction* evact=  new EventAction(fp,pFile,geotr,eventNumber); //Creates the Postsim class and sets branchs on the geotr. In general the Postsim is responsible for most output (except coordtr). Postsim also creates an instance of Qt.

  runManager->SetUserAction(evact);

  TrackingAction *trackingaction = new TrackingAction;
  StackingAction * stackaction = new StackingAction(); //coordtr created here
  runManager->SetUserAction(stackaction);
  runManager->SetUserAction(trackingaction);
  runManager->SetUserAction(new SteppingAction); //Creates a ttree called sttr, with position, momentum, and number of reflections
  TTree * coordtr = stackaction->GetCoordTr();


///////////////////////////////////
// Initialize G4 kernel, creates the detector and physics processes, preps for run

  runManager->Initialize(); // Calls Construct method of DetectorConstruction. Physics list is more complicated, calling G4VUserPhysicsLists as well as it's manager.

////////////////////////////////////////////////
  G4manager* UI = G4UImanager::GetUIpointer();
  G4String command = "/control/execute ";
  G4String fileName = argv[1];
  UI->ApplyCommand(command+fileName); 

  // job termination
  delete runManager;
  return 0;
}

