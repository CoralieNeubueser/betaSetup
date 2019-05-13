// from AnaEx01

#include "ActionInitialization.hh"
#include "HistoManager.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"

// #include "EventAction.hh"
// #include "SteppingAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ActionInitialization::ActionInitialization(DetectorConstruction* detector)
 : G4VUserActionInitialization(),
   fDetector(detector)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ActionInitialization::~ActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ActionInitialization::BuildForMaster() const
{
  // Histo manager
  HistoManager*  histo = new HistoManager();
  
  // Actions
  SetUserAction(new RunAction(histo));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ActionInitialization::Build() const
{
  // Histo manager
  HistoManager*  histo = new HistoManager();
  
  // Actions
  //
  SetUserAction(new PrimaryGeneratorAction(fDetector));
  
  RunAction* runAction = new RunAction(histo);  
  SetUserAction(runAction);
  
  // EventAction* eventAction = new EventAction(runAction, histo);
  // SetUserAction(eventAction);

  // SteppingAction* steppingAction = new SteppingAction(fDetector, eventAction);
  // SetUserAction(steppingAction);
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
