// from DetectorConstructionMessenger

#include "HistoManagerMessenger.hh"
#include "HistoManager.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManagerMessenger::HistoManagerMessenger(HistoManager* HM)
 : G4UImessenger(),
   fHistoManager(HM)
{
  // the /beta directory is already implemented by DetectorConstructionMessenger
  fHistoDirectory = new G4UIdirectory("/beta/histo/");
  fHistoDirectory->SetGuidance("HistoManager control");

  fFileNameCmd = new G4UIcmdWithAString("/beta/histo/outFileName",this);
  fFileNameCmd->SetGuidance("Output file name, without extension");
  fFileNameCmd->SetParameterName("outFileName",false);
  fFileNameCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManagerMessenger::~HistoManagerMessenger()
{
  delete fFileNameCmd;
  delete fHistoDirectory;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManagerMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{

  if( command == fFileNameCmd ) {
    fHistoManager->SetOutFileName(newValue);
  }   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
