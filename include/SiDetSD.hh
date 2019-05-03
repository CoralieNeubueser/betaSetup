// from B2a

#ifndef SIDETSD_HH
#define SIDETSD_HH

#include "G4VSensitiveDetector.hh"

#include "SiDetHit.hh"
#include "SpectrumHit.hh"

#include <vector>

class G4Step;
class G4HCofThisEvent;
class HistoManager;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/// SiDet sensitive detector class
///
/// The hits are accounted in hits in ProcessHits() function which is called
/// by Geant4 kernel at each step. A hit is created with each step with non zero 
/// energy deposit.

class SiDetSD : public G4VSensitiveDetector
{
public:
  SiDetSD(const G4String& name, 
	  G4int planeNumber);
  virtual ~SiDetSD();
  
  // methods from base class
  virtual void   Initialize(G4HCofThisEvent* hitCollection);
  virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
  virtual void   EndOfEvent(G4HCofThisEvent* hitCollection);

  void SetHistoManager(HistoManager* hm) {_histManager = hm;};
  
private:
  SiDetHitsCollection* fSiDetHC;
  SpectrumHitsCollection* fSpectrumHC;
  G4int _planeNum; // plane number to identify detector for histogramming 
  HistoManager* _histManager;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
