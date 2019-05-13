// from B2a

#include "SiDetSD.hh"
#include "HistoManager.hh"
#include "DetectorConstruction.hh"

#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

#include "HistoManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SiDetSD::SiDetSD(const G4String& name, G4int planeNumber) 
 : G4VSensitiveDetector(name),
   fSiDetHC(NULL),
   fSpectrumHC(NULL),
   _histManager(NULL)
{
  _planeNum = planeNumber;

  G4String hcName = name; // name for the hit collection, the SD name is unique
  hcName += "hitCollectionSiDet";
  collectionName.insert(hcName);

  hcName = name; // name for the hit collection, the SD name is unique
  hcName += "hitCollectionSpectrum";
  collectionName.insert(hcName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SiDetSD::~SiDetSD() 
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SiDetSD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection
  fSiDetHC = new SiDetHitsCollection(SensitiveDetectorName, collectionName[0]); 
  // Add this collection in hce
  G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fSiDetHC ); 

  // Create hits collection
  fSpectrumHC = new SpectrumHitsCollection(SensitiveDetectorName, collectionName[1]); 
  // Add this collection in hce
  hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[1]);
  hce->AddHitsCollection( hcID, fSpectrumHC ); 

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool SiDetSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{  
  // energy deposit
  G4double edep = aStep->GetTotalEnergyDeposit();
  //if (edep==0.) return false;

  // event ID
  const G4Event* evt = G4RunManager::GetRunManager()->GetCurrentEvent();
  if(!evt) return false;
  G4int eID = evt->GetEventID();

  const G4Track* track = aStep->GetTrack();

  if(edep > 0){
    SiDetHit* newHitSiDet = new SiDetHit();
    newHitSiDet->SetEvtID(eID);
    newHitSiDet->SetTrackID(track->GetTrackID());
    newHitSiDet->SetEdep(edep);
    newHitSiDet->SetPos(aStep->GetPreStepPoint()->GetPosition());
    newHitSiDet->SetPDGencoding(track->GetParticleDefinition()->GetPDGEncoding());
    newHitSiDet->SetEkin(track->GetKineticEnergy());
    newHitSiDet->SetTime(track->GetGlobalTime());
    fSiDetHC->insert( newHitSiDet );
  }
  
  if(aStep->IsFirstStepInVolume() == true){ // store info for particles entering the volume
    SpectrumHit* newHitSpectrum = new SpectrumHit();
    newHitSpectrum->SetEvtID(eID);
    newHitSpectrum->SetTrackID(track->GetTrackID());
    newHitSpectrum->SetEkin(track->GetKineticEnergy());
    newHitSpectrum->SetPDGencoding(track->GetParticleDefinition()->GetPDGEncoding());
    fSpectrumHC->insert( newHitSpectrum );
  }
  
  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SiDetSD::EndOfEvent(G4HCofThisEvent* hce)
{
  // check that at least one hit collection has some entries in this event
  // the loop looks into all hit collections
  G4int cap = hce->GetCapacity(); // number of "reserved places" in the hit collection vector, can contain a HC or not
  G4bool processEvt = false;
  for(G4int i = 0; i < cap; ++i){
    if(hce->GetHC(i)) // can return null
      if(hce->GetHC(i)->GetSize() > 0) // if there are entries in at least one HC, process evt (GetSize is used here as entries() does not belong to the base class)
	processEvt = true;
  }

  if(processEvt == false)
    return;
  
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance(); 

  G4double edep;
  G4double Etot = 0;
  G4double x = 0;
  G4double y = 0;
  G4double z = 0;
  G4double t = 0;
  G4int eID = -1;

  std::vector<G4int>* trkIDvec = _histManager->GetTrackNumVec(_planeNum);
  trkIDvec->clear();
  G4int trkID = -1;
  bool foundTrk = false;

  std::vector<G4int>* partVec = _histManager->GetParticleVec(_planeNum);
  partVec->clear();
  std::vector<G4double>* ekinVec = _histManager->GetEkinVec(_planeNum);
  ekinVec->clear();
  std::vector<G4double>* edepVec = _histManager->GetEdepVec(_planeNum);
  edepVec->clear();
  std::vector<G4double>* xVec = _histManager->GetXVec(_planeNum);
  xVec->clear();
  std::vector<G4double>* yVec = _histManager->GetYVec(_planeNum);
  yVec->clear();
  std::vector<G4double>* zVec = _histManager->GetZVec(_planeNum);
  zVec->clear();
  std::vector<G4double>* tVec = _histManager->GetTVec(_planeNum);
  tVec->clear();
  std::vector<G4int> countHitVec; // vector to count how many hits each track has, to have average time
  
  G4int nofHits = fSiDetHC->entries();
  if(nofHits > 0){
    for(G4int i= 0; i < nofHits; ++i){
      edep = (*fSiDetHC)[i]->GetEdep();
      
      trkID = (*fSiDetHC)[i]->GetTrackID();
      foundTrk = false;
      for(G4int itrk = 0; itrk < (G4int) trkIDvec->size(); itrk++) // check if the track is already in the vector
	if(trkID == trkIDvec->at(itrk)){
	  foundTrk = true;

	  // if track is there, add up quantities
	  edepVec->at(itrk) += edep;
	  xVec->at(itrk) += (*fSiDetHC)[i]->GetPos()[0] * edep;
	  yVec->at(itrk) += (*fSiDetHC)[i]->GetPos()[1] * edep;
	  zVec->at(itrk) += (*fSiDetHC)[i]->GetPos()[2] * edep;
	  tVec->at(itrk) += (*fSiDetHC)[i]->GetTime();
	  countHitVec.at(itrk) += 1;
	}
      
      if(foundTrk == false){ // new track, push back stuff
	trkIDvec->push_back(trkID);
	partVec->push_back( (*fSiDetHC)[i]->GetPDGencoding() );
	ekinVec->push_back( (*fSiDetHC)[i]->GetEkin() ); // the first hit found in the HC is used for the kinetic energy
	edepVec->push_back(edep);
	xVec->push_back( (*fSiDetHC)[i]->GetPos()[0] * edep );
	yVec->push_back( (*fSiDetHC)[i]->GetPos()[1] * edep );
	zVec->push_back( (*fSiDetHC)[i]->GetPos()[2] * edep );
	tVec->push_back( (*fSiDetHC)[i]->GetTime() );
	countHitVec.push_back(1);
      }
	

      // calculate quantities for the tree
      Etot += edep;
      x += (*fSiDetHC)[i]->GetPos()[0] * edep;
      y += (*fSiDetHC)[i]->GetPos()[1] * edep;
      z += (*fSiDetHC)[i]->GetPos()[2] * edep;
      t += (*fSiDetHC)[i]->GetTime();
    }
    x /= Etot;
    y /= Etot;
    z /= Etot;
    t /= nofHits;
    
    for(G4int itrk = 0; itrk < (G4int) trkIDvec->size(); itrk++){
      xVec->at(itrk) /= edepVec->at(itrk);
      yVec->at(itrk) /= edepVec->at(itrk);
      zVec->at(itrk) /= edepVec->at(itrk);
      tVec->at(itrk) /= countHitVec.at(itrk);
    }

    // fill histograms
    analysisManager->FillH1(_planeNum, Etot);
    analysisManager->FillH2(_planeNum, x, y);
    eID = (*fSiDetHC)[0]->GetEvtID();// event id from hit collection
  } // nofHits > 0
  else{
    Etot = -5;
    x = -999;
    y = -999;
    z = -999;
    t = -999;
    // event ID from event manager
    const G4Event* evt = G4RunManager::GetRunManager()->GetCurrentEvent();
    eID = evt->GetEventID();
  }
  
  // fill tree with summed or averaged quantities for this plane
  analysisManager->FillNtupleIColumn(_planeNum, 0, eID);
  analysisManager->FillNtupleDColumn(_planeNum, 1, Etot);
  analysisManager->FillNtupleIColumn(_planeNum, 2, nofHits);
  analysisManager->FillNtupleDColumn(_planeNum, 3, x);
  analysisManager->FillNtupleDColumn(_planeNum, 4, y);
  analysisManager->FillNtupleDColumn(_planeNum, 5, z);
  analysisManager->FillNtupleDColumn(_planeNum, 6, t);
  analysisManager->FillNtupleIColumn(_planeNum, 7, trkIDvec->size());
  analysisManager->AddNtupleRow(_planeNum);  

  nofHits = fSpectrumHC->entries();
  if(nofHits > 0)
    for(G4int i= 0; i < nofHits; ++i){
      G4int pdgenc = (*fSpectrumHC)[i]->GetPDGencoding();
      G4double ekin = (*fSpectrumHC)[i]->GetEkin();
      // fill histograms
      if(pdgenc == 11 || pdgenc == -11) // electron or positron
  	analysisManager->FillH1(_planeNum + DetectorConstruction::nPlanes, ekin);
      else if(pdgenc == 22) // gamma
  	analysisManager->FillH1(_planeNum + 2*DetectorConstruction::nPlanes, ekin);
    }
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
