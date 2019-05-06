// from AnaEx01

#include "HistoManager.hh"
#include "HistoManagerMessenger.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "DetectorConstruction.hh" // to get number of planes

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::HistoManager()
 : fFactoryOn(false)
{
  fOutFileName = "beta";
  fMessenger = new HistoManagerMessenger(this);

  _trackNumVec = new std::vector<G4int>*[DetectorConstruction::nPlanes];
  _particleVec = new std::vector<G4int>*[DetectorConstruction::nPlanes];
  _ekinVec = new std::vector<G4double>*[DetectorConstruction::nPlanes];
  _edepVec = new std::vector<G4double>*[DetectorConstruction::nPlanes];
  _xVec = new std::vector<G4double>*[DetectorConstruction::nPlanes];
  _yVec = new std::vector<G4double>*[DetectorConstruction::nPlanes];
  _zVec = new std::vector<G4double>*[DetectorConstruction::nPlanes];

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::~HistoManager()
{
  delete[] _trackNumVec;
  delete[] _particleVec;
  delete[] _ekinVec;
  delete[] _edepVec;
  delete[] _xVec;
  delete[] _yVec;
  delete[] _zVec;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Book()
{
  // Create or get analysis manager
  // The choice of analysis technology is done via selection of a namespace
  // in HistoManager.hh
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetNtupleMerging(true);
      
  // Create directories 
  analysisManager->SetHistoDirectoryName("histograms");
  analysisManager->SetNtupleDirectoryName("trees");
    
  // Open an output file
  //
  G4bool fileOpen = analysisManager->OpenFile(fOutFileName);
  if (! fileOpen) {
    G4cerr << "\n---> HistoManager::Book(): cannot open " 
           << analysisManager->GetFileName() << G4endl;
    return;
  }
  
  // Create histograms.
  // Histogram ids are generated automatically starting from 0.
  // The start value can be changed by:
  // analysisManager->SetFirstHistoId(1);  

  G4String name;
  for(G4int i = 0; i < DetectorConstruction::nPlanes; ++i){ // ID: 0, ..., nPlanes - 1
    name = "EdepPlane";
    name += std::to_string(i);
    analysisManager->CreateH1(name,"Deposited energy in Silicon", 1000, 0., 0.2*MeV);
    analysisManager->SetH1XAxisTitle(i, "Energy [MeV]");
    analysisManager->SetH1YAxisTitle(i, "Entries");
  }

  // 2D histos get different numbering
  for(G4int i = 0; i < DetectorConstruction::nPlanes; ++i){ // ID: 0, ..., nPlanes - 1
    name = "HitMapPlane";
    name += std::to_string(i);
    analysisManager->CreateH2(name,"HitMap", 200, -50*mm, 50*mm, 200, -50*mm, 50*mm);
    analysisManager->SetH2XAxisTitle(i, "X [mm]");
    analysisManager->SetH2YAxisTitle(i, "Y [mm]");
    analysisManager->SetH2ZAxisTitle(i, "Entries");
  }
  
  for(G4int i = 0; i < DetectorConstruction::nPlanes; ++i){ // ID: nPlanes, ..., 2*nPlanes - 1
    name = "ElectronPositronSpectrumPlane";
    name += std::to_string(i);
    analysisManager->CreateH1(name,"e^{-} e^{+} spectrum", 2000, 0., 2.9*MeV);
    analysisManager->SetH1XAxisTitle(i + DetectorConstruction::nPlanes, "Energy [MeV]");
    analysisManager->SetH1YAxisTitle(i + DetectorConstruction::nPlanes, "Entries");
  }

  for(G4int i = 0; i < DetectorConstruction::nPlanes; ++i){ // ID: 2*nPlanes, ..., 3*nPlanes - 1
    name = "GammaSpectrumPlane";
    name += std::to_string(i);
    analysisManager->CreateH1(name,"#gamma spectrum", 2000, 0., 2.9*MeV);
    analysisManager->SetH1XAxisTitle(i + 2*DetectorConstruction::nPlanes, "Energy [MeV]");
    analysisManager->SetH1YAxisTitle(i + 2*DetectorConstruction::nPlanes, "Entries");
  }

  // Create ntuples.
  // Ntuples ids are generated automatically starting from 0.
  // The start value can be changed by:
  // analysisManager->SetFirstMtupleId(1);  
  
  for(G4int i = 0; i < DetectorConstruction::nPlanes; ++i){ // ID: 0, ..., nPlanes - 1
    name = "EdepTreePlane";
    name += std::to_string(i);
    analysisManager->CreateNtuple(name, "Etot and summed or averaged quantities");
    analysisManager->CreateNtupleIColumn("event"); // column Id = 0
    analysisManager->CreateNtupleDColumn("Etot"); // column Id = 1
    analysisManager->CreateNtupleIColumn("nHits"); // column Id = 2
    analysisManager->CreateNtupleDColumn("x"); // column Id = 3
    analysisManager->CreateNtupleDColumn("y"); // column Id = 4
    analysisManager->CreateNtupleDColumn("z"); // column Id = 5
    analysisManager->CreateNtupleIColumn("nTracks"); // column Id = 6
    _trackNumVec[i] = new std::vector<G4int>;
    analysisManager->CreateNtupleIColumn("trackNumVec", *_trackNumVec[i]); // column Id = 7
    _particleVec[i] = new std::vector<G4int>;
    analysisManager->CreateNtupleIColumn("particleVec", *_particleVec[i]); // column Id = 8
    _ekinVec[i] = new std::vector<G4double>;
    analysisManager->CreateNtupleDColumn("ekinVec", *_ekinVec[i]); // column Id = 9
    _edepVec[i] = new std::vector<G4double>;
    analysisManager->CreateNtupleDColumn("edepVec", *_edepVec[i]); // column Id = 10
    _xVec[i] = new std::vector<G4double>;
    analysisManager->CreateNtupleDColumn("xVec", *_xVec[i]); // column Id = 11
    _yVec[i] = new std::vector<G4double>;
    analysisManager->CreateNtupleDColumn("yVec", *_yVec[i]); // column Id = 12
    _zVec[i] = new std::vector<G4double>;
    analysisManager->CreateNtupleDColumn("zVec", *_zVec[i]); // column Id = 13
    analysisManager->FinishNtuple();
  }

  
  fFactoryOn = true;       

  G4cout << "\n----> Output file is open in " 
         << analysisManager->GetFileName() << "." 
         << analysisManager->GetFileType() << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Save()
{
  if (! fFactoryOn) return;
  
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();    
  analysisManager->Write();
  analysisManager->CloseFile(); 
   
  G4cout << "\n----> Histograms and ntuples are saved\n" << G4endl;
      
  delete G4AnalysisManager::Instance();
  fFactoryOn = false;
}

