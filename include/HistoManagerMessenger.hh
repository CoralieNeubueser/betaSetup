// from DetectorConstructionMessenger

#ifndef HISTOMANAGERMESSENGER_HH
#define HISTOMANAGERMESSENGER_HH

#include "globals.hh"
#include "G4UImessenger.hh"

class HistoManager;
class G4UIdirectory;
class G4UIcmdWithAString;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/// Messenger class that defines commands for HistoManager

class HistoManagerMessenger: public G4UImessenger
{
public:
  HistoManagerMessenger(HistoManager* );
  virtual ~HistoManagerMessenger();
    
  virtual void SetNewValue(G4UIcommand*, G4String);
    
private:
  HistoManager* fHistoManager;

  // the /beta directory is already defined
  G4UIdirectory* fHistoDirectory;

  G4UIcmdWithAString* fFileNameCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
