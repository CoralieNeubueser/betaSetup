// from B2a

#include "DetectorMessenger.hh"
#include "DetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::DetectorMessenger(DetectorConstruction* Det)
 : G4UImessenger(),
   fDetectorConstruction(Det)
{
  // directory for program
  fBetaDirectory = new G4UIdirectory("/beta/");
  fBetaDirectory->SetGuidance("UI commands specific to this program.");

  // directory for detector construction
  fDetDirectory = new G4UIdirectory("/beta/det/");
  fDetDirectory->SetGuidance("Detector construction control");

  // Reinitialize command
  fReinitializeCmd = new G4UIcmdWithoutParameter("/beta/det/Reinitialize",this);
  fReinitializeCmd->SetGuidance("Reinitialize geometry. Call after setting all needed stuff");
  fReinitializeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
  G4String name;

  // silicon thickness
  fSiThickCmd = new G4UIcmdWithADoubleAndUnit*[DetectorConstruction::nPlanes];
  for(G4int i = 0; i < DetectorConstruction::nPlanes; ++i){
    name = "/beta/det/siThick";
    name += std::to_string(i);
    fSiThickCmd[i] = new G4UIcmdWithADoubleAndUnit(name,this);
    name = "Define the thickness of Si on plane ";
    name += std::to_string(i);
    name += ". Thickness can not be 0.";
    fSiThickCmd[i]->SetGuidance(name);
    fSiThickCmd[i]->SetParameterName("Thickness",false);
    fSiThickCmd[i]->SetUnitCategory("Length");
    fSiThickCmd[i]->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  // silicon side
  fSiSideCmd = new G4UIcmdWithADoubleAndUnit*[DetectorConstruction::nPlanes];
  for(G4int i = 0; i < DetectorConstruction::nPlanes; ++i){
    name = "/beta/det/siSide";
    name += std::to_string(i);
    fSiSideCmd[i] = new G4UIcmdWithADoubleAndUnit(name,this);
    name = "Define the side of Si on plane ";
    name += std::to_string(i);
    name += ". Side can not be 0.";
    fSiSideCmd[i]->SetGuidance(name);
    fSiSideCmd[i]->SetParameterName("Side",false);
    fSiSideCmd[i]->SetUnitCategory("Length");
    fSiSideCmd[i]->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  // plane distance
  fPlaneDistCmd = new G4UIcmdWithADoubleAndUnit*[DetectorConstruction::nPlanes];
  for(G4int i = 0; i < DetectorConstruction::nPlanes; ++i){
    name = "/beta/det/planeDist";
    name += std::to_string(i);
    fPlaneDistCmd[i] = new G4UIcmdWithADoubleAndUnit(name,this);
    name = "Define distance of silicon top surface on plane ";
    name += std::to_string(i);
    name += " from previous plane.";
    fPlaneDistCmd[i]->SetGuidance(name);
    fPlaneDistCmd[i]->SetParameterName("Distance",false);
    fPlaneDistCmd[i]->SetUnitCategory("Length");
    fPlaneDistCmd[i]->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  // collimator thickness
  fCollThickCmd = new G4UIcmdWithADoubleAndUnit("/beta/det/collThick",this);
  fCollThickCmd->SetGuidance("Set collimator thickness. Set to 0 to remove collimator from geometry.");
  fCollThickCmd->SetParameterName("Thickness",false);
  fCollThickCmd->SetUnitCategory("Length");
  fCollThickCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  // collimator hole diameter
  fCollHoleDiamCmd = new G4UIcmdWithADoubleAndUnit("/beta/det/collHoleDiam",this);
  fCollHoleDiamCmd->SetGuidance("Set collimator hole diameter.");
  fCollHoleDiamCmd->SetParameterName("Hole diameter",false);
  fCollHoleDiamCmd->SetUnitCategory("Length");
  fCollHoleDiamCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  // absorber thickness
  fAbsThickCmd = new G4UIcmdWithADoubleAndUnit("/beta/det/absThick",this);
  fAbsThickCmd->SetGuidance("Set absorber thickness. Set to 0 to remove absorber from geometry.");
  fAbsThickCmd->SetParameterName("Thickness",false);
  fAbsThickCmd->SetUnitCategory("Length");
  fAbsThickCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  // absorber material
  fAbsMatCmd = new G4UIcmdWithAString("/beta/det/absMat",this);
  fAbsMatCmd->SetGuidance("Set absorber material.");
  fAbsMatCmd->SetParameterName("Materials",false);
  fAbsMatCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  // pcb substrate thickness
  fPcbSubThickCmd = new G4UIcmdWithADoubleAndUnit("/beta/det/pcbSubThick",this);
  fPcbSubThickCmd->SetGuidance("Set PCB substrate thickness. Set to 0 to remove the PCBs from geometry.");
  fPcbSubThickCmd->SetParameterName("Thickness",false);
  fPcbSubThickCmd->SetUnitCategory("Length");
  fPcbSubThickCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  // pcb metal thickness
  fPcbMetalThickCmd = new G4UIcmdWithADoubleAndUnit("/beta/det/pcbMetalThick",this);
  fPcbMetalThickCmd->SetGuidance("Set PCB metal thickness. A metal layer is present on both PCB sides. Set to 0 to remove the PCBs from geometry.");
  fPcbMetalThickCmd->SetParameterName("Thickness",false);
  fPcbMetalThickCmd->SetUnitCategory("Length");
  fPcbMetalThickCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  // pcb side
  fPcbSideCmd = new G4UIcmdWithADoubleAndUnit("/beta/det/pcbSide",this);
  fPcbSideCmd->SetGuidance("Set PCB side. Side can not be 0.");
  fPcbSideCmd->SetParameterName("Side",false);
  fPcbSideCmd->SetUnitCategory("Length");
  fPcbSideCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::~DetectorMessenger()
{
  delete fReinitializeCmd;
  delete[] fSiThickCmd;
  delete[] fSiSideCmd;
  delete[] fPlaneDistCmd;
  delete fCollThickCmd;
  delete fCollHoleDiamCmd;
  delete fAbsThickCmd;
  delete fAbsMatCmd;
  delete fPcbSubThickCmd;
  delete fPcbMetalThickCmd;
  delete fPcbSideCmd;
  delete fBetaDirectory;
  delete fDetDirectory;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  // silicon thickness
  for(G4int i = 0; i < DetectorConstruction::nPlanes; ++i)
    if( command == fSiThickCmd[i] )
      fDetectorConstruction->SetSiThickness(i, fSiThickCmd[i]->GetNewDoubleValue(newValue));

  // silicon side
  for(G4int i = 0; i < DetectorConstruction::nPlanes; ++i)
    if( command == fSiSideCmd[i] )
      fDetectorConstruction->SetSiSide(i, fSiSideCmd[i]->GetNewDoubleValue(newValue));

  // plane distance
  for(G4int i = 0; i < DetectorConstruction::nPlanes; ++i)
    if( command == fPlaneDistCmd[i] )
      fDetectorConstruction->SetPlaneDistance(i, fPlaneDistCmd[i]->GetNewDoubleValue(newValue));

  // collimator thickness
  if( command == fCollThickCmd )
      fDetectorConstruction->SetCollimatorThickness(fCollThickCmd->GetNewDoubleValue(newValue));

  // collimator hole diameter
  if( command == fCollHoleDiamCmd )
      fDetectorConstruction->SetCollimatorHoleDiameter(fCollHoleDiamCmd->GetNewDoubleValue(newValue));

  // absorber thickness
  if( command == fAbsThickCmd )
      fDetectorConstruction->SetAbsorberThickness(fAbsThickCmd->GetNewDoubleValue(newValue));

  // absorber material
  if( command == fAbsMatCmd )
      fDetectorConstruction->SetAbsorberMaterial(newValue);

  // pcb substrate thickness
  if( command == fPcbSubThickCmd )
      fDetectorConstruction->SetPcbSubThickness(fPcbSubThickCmd->GetNewDoubleValue(newValue));

  // pcb metal thickness
  if( command == fPcbMetalThickCmd )
      fDetectorConstruction->SetPcbMetalThickness(fPcbMetalThickCmd->GetNewDoubleValue(newValue));

  // pcb side
  if( command == fPcbSideCmd )
      fDetectorConstruction->SetPcbSide(fPcbSideCmd->GetNewDoubleValue(newValue));

  // reinitialize
  if( command == fReinitializeCmd )
    fDetectorConstruction->Reinitialize();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
