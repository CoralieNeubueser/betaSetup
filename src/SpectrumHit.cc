// from B2a

#include "SpectrumHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>

G4ThreadLocal G4Allocator<SpectrumHit>* SpectrumHitAllocator=0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SpectrumHit::SpectrumHit()
 : G4VHit(),
   fTrackID(-1),
   fEkin(0.),
   fPDGencoding(0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SpectrumHit::~SpectrumHit() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SpectrumHit::SpectrumHit(const SpectrumHit& right)
  : G4VHit()
{
  fTrackID   = right.fTrackID;
  fEkin      = right.fEkin;
  fPDGencoding       = right.fPDGencoding;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const SpectrumHit& SpectrumHit::operator=(const SpectrumHit& right)
{
  fTrackID   = right.fTrackID;
  fEkin      = right.fEkin;
  fPDGencoding       = right.fPDGencoding;

  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int SpectrumHit::operator==(const SpectrumHit& right) const
{
  return ( this == &right ) ? 1 : 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SpectrumHit::Draw()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SpectrumHit::Print()
{
  G4cout
     << "  trackID: " << fTrackID
     << "Ekin: "
     << std::setw(7) << G4BestUnit(fEkin,"Energy")
     << " PDG encoding: "
     << std::setw(7) << fPDGencoding
     << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
