// from testEM8 and B3

#ifndef PHYSICSLIST_HH
#define PHYSICSLIST_HH

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class G4VPhysicsConstructor;
class PhysicsListMessenger;
class DetectorConstruction;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PhysicsList: public G4VModularPhysicsList
{
public:

  PhysicsList();
  virtual ~PhysicsList();

  //virtual void ConstructParticle();
  //virtual void ConstructProcess(); 
  virtual void SetCuts();
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
