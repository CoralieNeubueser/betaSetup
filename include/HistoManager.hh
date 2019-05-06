// from AnaEx01

#ifndef HISTOMANAGER_HH
#define HISTOMANAGER_HH

#include "globals.hh"

#include "g4root.hh"
//#include "g4csv.hh"
//#include "g4xml.hh"

class HistoManagerMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class HistoManager
{
public:
  HistoManager();
  ~HistoManager();

  void Book();
  void Save();

  void SetOutFileName(G4String name) {fOutFileName = name;};

  std::vector<G4int>* GetTrackNumVec(G4int i) {return _trackNumVec[i];};
  std::vector<G4int>* GetParticleVec(G4int i) {return _particleVec[i];};
  std::vector<G4double>* GetEdepVec(G4int i) {return _edepVec[i];};
  std::vector<G4double>* GetXVec(G4int i) {return _xVec[i];};
  std::vector<G4double>* GetYVec(G4int i) {return _yVec[i];};
  std::vector<G4double>* GetZVec(G4int i) {return _zVec[i];};
  
private:
  G4bool fFactoryOn;    
  G4String fOutFileName;
  HistoManagerMessenger* fMessenger;

  // vectors to be used in the trees
  std::vector<G4int>** _trackNumVec; 
  std::vector<G4int>** _particleVec; 
  std::vector<G4double>** _edepVec;
  std::vector<G4double>** _xVec;
  std::vector<G4double>** _yVec;
  std::vector<G4double>** _zVec;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

