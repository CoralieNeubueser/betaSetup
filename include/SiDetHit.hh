// from B2a

#ifndef SIDETHIT_HH
#define SIDETHIT_HH

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"

/// hit class
///
/// It defines data members to store the trackID, energy deposit,
/// and position of charged particles in a selected volume:
/// - fTrackID, fEdep, fPos

class SiDetHit : public G4VHit
{
public:
  SiDetHit();
  SiDetHit(const SiDetHit&);
  virtual ~SiDetHit();

  // operators
  const SiDetHit& operator=(const SiDetHit&);
  G4int operator==(const SiDetHit&) const;

  inline void* operator new(size_t);
  inline void  operator delete(void*);

  // methods from base class
  virtual void Draw();
  virtual void Print();

  // Set methods
  void SetEvtID    (G4int evt)        { fEvtID = evt; };
  void SetTrackID  (G4int track)      { fTrackID = track; };
  void SetEdep     (G4double de)      { fEdep = de; };
  void SetPos      (G4ThreeVector xyz){ fPos = xyz; };
  void SetPDGencoding      (G4int enc){ fPDGencoding = enc; };
  void SetEkin     (G4double de)      { fEkin = de; };

  // Get methods
  G4int GetEvtID() const       { return fEvtID; };
  G4int GetTrackID() const     { return fTrackID; };
  G4double GetEdep() const     { return fEdep; };
  G4ThreeVector GetPos() const { return fPos; };
  G4int GetPDGencoding() const { return fPDGencoding; };
  G4double GetEkin() const     { return fEkin; };

private:
  
  G4int         fEvtID;  
  G4int         fTrackID;
  G4double      fEdep;
  G4ThreeVector fPos;
  G4int fPDGencoding;
  G4double      fEkin;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<SiDetHit> SiDetHitsCollection;

extern G4ThreadLocal G4Allocator<SiDetHit>* SiDetHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* SiDetHit::operator new(size_t)
{
  if(!SiDetHitAllocator)
      SiDetHitAllocator = new G4Allocator<SiDetHit>;
  return (void *) SiDetHitAllocator->MallocSingle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void SiDetHit::operator delete(void *hit)
{
  SiDetHitAllocator->FreeSingle((SiDetHit*) hit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
