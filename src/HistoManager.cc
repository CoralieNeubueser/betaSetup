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
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::~HistoManager()
{}

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

