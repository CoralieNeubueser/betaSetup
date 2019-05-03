// from AnaEx01

#include "RunAction.hh"
#include "HistoManager.hh"

#include "DetectorConstruction.hh"
#include "SiDetSD.hh"

#include "G4SDManager.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(HistoManager* histo)
: G4UserRunAction(),
  fHistoManager(histo)
{
  return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
  return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* aRun)
{ 
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

  // set the histo manager pointer for the sensitive detectors so that they can access the vectors
  G4String name;
  for(G4int i = 0; i < DetectorConstruction::nPlanes; ++i){
    name = "siSensDet";
    name += std::to_string(i);
    SiDetSD* det = (SiDetSD*) G4SDManager::GetSDMpointer()->FindSensitiveDetector(name, true);
    det->SetHistoManager(fHistoManager);
  }
  
  //histograms
  //
  fHistoManager->Book(); 
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* aRun)
{
  G4int NbOfEvents = aRun->GetNumberOfEvent();
  if (NbOfEvents == 0) return;
  
     
  //save histograms
  //
  //fHistoManager->PrintStatistic();
  fHistoManager->Save();   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
