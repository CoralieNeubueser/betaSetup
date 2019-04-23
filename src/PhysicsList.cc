// from testEM8 and B3

#include "PhysicsList.hh"

#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"

#include "G4EmPenelopePhysics.hh"
#include "G4EmParameters.hh"

#include "G4PAIModel.hh"
#include "G4PAIPhotModel.hh"

#include "G4SystemOfUnits.hh"
#include "G4StepLimiterPhysics.hh"

// from rdecay01
#include "G4ParticleTypes.hh"
#include "G4IonConstructor.hh"
#include "G4PhysicsListHelper.hh"
#include "G4RadioactiveDecay.hh"
#include "G4NuclideTable.hh"
#include "G4LossTableManager.hh"
#include "G4UAtomicDeexcitation.hh"
#include "G4NuclearLevelData.hh"
#include "G4DeexPrecoParameters.hh"
#include "G4NuclideTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::PhysicsList() 
  : G4VModularPhysicsList()
{
  // SetVerboseLevel(1);

  // set cuts for EM processes (gamma, e-, e+, proton)
  defaultCutValue = 1 * um;
  
  // Default physics
  RegisterPhysics(new G4DecayPhysics());
  
  // EM physics
  RegisterPhysics(new G4EmPenelopePhysics());
  
  // add PAI model
  G4EmParameters::Instance()->AddPAIModel("all","World","pai");
  
  // // enabling this does not seem to have an effect on Edep distribution
  // G4EmParameters::Instance()->AddPAIModel("all","World","pai_photon");
  
  // Radioactive decay
  RegisterPhysics(new G4RadioactiveDecayPhysics());
  
  // use to put step limits in interesting volumes
  RegisterPhysics(new G4StepLimiterPhysics());
  

  // from rdecay01
  // mandatory for G4NuclideTable
  //
  G4NuclideTable::GetInstance()->SetThresholdOfHalfLife(0.1*picosecond);
  G4NuclideTable::GetInstance()->SetLevelTolerance(1.0*eV);
  //read new PhotonEvaporation data set 
  //
  G4DeexPrecoParameters* deex = 
    G4NuclearLevelData::GetInstance()->GetParameters();
  deex->SetCorrelatedGamma(false);
  deex->SetStoreAllLevels(true);
  deex->SetMaxLifeTime(G4NuclideTable::GetInstance()->GetThresholdOfHalfLife()/std::log(2.));
  
  return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::~PhysicsList()
{
  return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetCuts()
{
  //G4VUserPhysicsList::SetCuts(); // from B3

  // use the cut defined in the constructor (defaultCutValue)
  SetCutsWithDefault();

  // lower the energy edge for production cuts, does not seem to affect the energy deposit in silicon from charged particles
  // fluorescence x-rays are enhanched when this is active
  //G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(250*eV, 1*GeV);
  
  return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

