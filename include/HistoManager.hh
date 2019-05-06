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

  std::vector<G4int>* GetTrackNumVec(G4int i) {return trackNumVec[i];};
  
private:
  G4bool fFactoryOn;    
  G4String fOutFileName;
  HistoManagerMessenger* fMessenger;
  std::vector<G4int>** trackNumVec;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

