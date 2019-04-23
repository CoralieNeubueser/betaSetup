// from AnaEx01

#include "PrimaryGeneratorAction.hh"

#include "DetectorConstruction.hh"

#include "G4PhysicalConstants.hh"
#include "Randomize.hh"
#include "G4IonTable.hh"
#include "G4Geantino.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction(DetectorConstruction* DC)
: G4VUserPrimaryGeneratorAction(),
  fParticleGun(0),
  fDetector(DC)
{
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);

  // default particle kinematic

  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.)); // keep direction in case of test with muons
  fParticleGun->SetParticleEnergy(0.*eV);
 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //this function is called at the begining of event
  // 

  // from rdecay01
  if (fParticleGun->GetParticleDefinition() == G4Geantino::Geantino()) { // if no particle is set
    // 90Sr is default
    G4int Z = 38;
    G4int A = 90;
    G4ParticleDefinition* ion = G4IonTable::GetIonTable()->GetIon(Z,A,0*eV);
    fParticleGun->SetParticleDefinition(ion);
    //fParticleGun->SetParticleCharge(0*eplus);
  }
  
  // the particles are generated in a sphere centered at (0,0,position)
  // the sphere radius is radius
  G4double position = fDetector->GetSourcePosZ();
  G4double radius = fDetector->GetSourceRadius();
  G4double r = G4UniformRand() * radius;
  G4double theta = G4UniformRand() * pi;
  G4double phi = G4UniformRand() * 2*pi;
  G4double x = r * sin(theta) * cos(phi);
  G4double y = r * sin(theta) * sin(phi);
  G4double z = r * cos(theta) + position;
  fParticleGun->SetParticlePosition(G4ThreeVector(x,y,z));

  fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

