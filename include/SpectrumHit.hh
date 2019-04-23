// from B2a

#ifndef SPECTRUMHIT_HH
#define SPECTRUMHIT_HH

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"

/// hit class
///
/// It defines data members to store the trackID, energy deposit,
/// and position of charged particles in a selected volume:
/// - fTrackID, fEkin, fPos

class SpectrumHit : public G4VHit
{
public:
  SpectrumHit();
  SpectrumHit(const SpectrumHit&);
  virtual ~SpectrumHit();

  // operators
  const SpectrumHit& operator=(const SpectrumHit&);
  G4int operator==(const SpectrumHit&) const;

  inline void* operator new(size_t);
  inline void  operator delete(void*);

  // methods from base class
  virtual void Draw();
  virtual void Print();

  // Set methods
  void SetEvtID    (G4int evt)        { fEvtID = evt; };
  void SetTrackID  (G4int track)      { fTrackID = track; };
  void SetEkin     (G4double de)      { fEkin = de; };
  void SetPDGencoding      (G4int enc){ fPDGencoding = enc; };

  // Get methods
  G4int GetEvtID() const       { return fEvtID; };
  G4int GetTrackID() const     { return fTrackID; };
  G4double GetEkin() const     { return fEkin; };
  G4int GetPDGencoding() const { return fPDGencoding; };

private:

  G4int         fEvtID;  
  G4int         fTrackID;
  G4double      fEkin;
  G4int fPDGencoding;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<SpectrumHit> SpectrumHitsCollection;

extern G4ThreadLocal G4Allocator<SpectrumHit>* SpectrumHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* SpectrumHit::operator new(size_t)
{
  if(!SpectrumHitAllocator)
      SpectrumHitAllocator = new G4Allocator<SpectrumHit>;
  return (void *) SpectrumHitAllocator->MallocSingle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void SpectrumHit::operator delete(void *hit)
{
  SpectrumHitAllocator->FreeSingle((SpectrumHit*) hit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
