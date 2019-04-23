// from B2a

#ifndef DETECTORMESSENGER_HH
#define DETECTORMESSENGER_HH

#include "globals.hh"
#include "G4UImessenger.hh"

class DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;
class G4UIcmdWithoutParameter;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/// Messenger class that defines commands for DetectorConstruction.
///
/// It implements commands:
/// /beta/det/siThick# value unit

class DetectorMessenger: public G4UImessenger
{
public:
  DetectorMessenger(DetectorConstruction* );
  virtual ~DetectorMessenger();
    
  virtual void SetNewValue(G4UIcommand*, G4String);
    
private:
  DetectorConstruction*  fDetectorConstruction;

  G4UIdirectory*           fBetaDirectory;
  G4UIdirectory*           fDetDirectory;

  // set si thickness
  // pointer to pointer to create an array of commands
  G4UIcmdWithADoubleAndUnit** fSiThickCmd;
  // set si side
  // pointer to pointer to create an array of commands
  G4UIcmdWithADoubleAndUnit** fSiSideCmd;
  // set planes distance
  // pointer to pointer to create an array of commands
  G4UIcmdWithADoubleAndUnit** fPlaneDistCmd;

  // set collimator thickness
  G4UIcmdWithADoubleAndUnit* fCollThickCmd;
  // set collimator hole diameter
  G4UIcmdWithADoubleAndUnit* fCollHoleDiamCmd;

  // set absorber thickness
  G4UIcmdWithADoubleAndUnit* fAbsThickCmd;
  // set absorber material
  G4UIcmdWithAString* fAbsMatCmd;

  // set PCB substrate thickness
  G4UIcmdWithADoubleAndUnit* fPcbSubThickCmd;
  // set PCB metal thickness
  G4UIcmdWithADoubleAndUnit* fPcbMetalThickCmd;
  // set PCB side
  G4UIcmdWithADoubleAndUnit* fPcbSideCmd;
  
  // reinitialize geometry
  G4UIcmdWithoutParameter* fReinitializeCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
