// modified from example B1

#ifndef DETECTORCONSTRUCTION_HH
#define DETECTORCONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class DetectorMessenger;
class G4Box;
class G4Region;
class G4Material;

/// Detector construction class to define materials and geometry.

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
  DetectorConstruction();
  virtual ~DetectorConstruction();
  
  virtual G4VPhysicalVolume* Construct();
  virtual void ConstructSDandField();
  
  G4double GetSourcePosZ() {return _sourcePosZ;};
  G4double GetSourceRadius() {return _sourceRadius;};

  // setters used by commands
  void SetSiThickness(G4int index, G4double thickness);
  void SetSiSide(G4int index, G4double side);
  void SetPlaneDistance(G4int index, G4double distance);

  void SetCollimatorThickness(G4double t) {_collThickness = t;};
  void SetCollimatorHoleDiameter(G4double d) {_collHoleDiam = d;};

  void SetAbsorberMaterial(G4String mat) {_absorberMaterial = mat;};
  void SetAbsorberThickness(G4double t) {_absorberThickness = t;};

  void SetPcbSubThickness(G4double t) {_pcbSubThickness = t;};
  void SetPcbMetalThickness(G4double t) {_pcbMetalThickness = t;};
  void SetPcbSide(G4double t) {_pcbSide = t;};

  // command to reinitialize the geometry
  void Reinitialize();
  
  static int nPlanes; // number of planes (Si plus PCB), may be useful to other classes

  private:
  // world dimensions (it is a cube)
  G4double _worldSide;

  // source casing
  G4double _sourceCaseDiam; // diameter
  G4double _sourceCaseHeight; // height
  
  // source position for PrimaryGenerationAction
  G4double _sourcePosZ; // radioactive material position along z
  G4double _sourceRadius; // radius of the radioactive material

  // // collimator (diameter same as source casing, located just after source)
  G4double _collThickness;
  G4double _collHoleDiam;

  // // absorber (diameter same as source casing, located just after collimator)
  G4String _absorberMaterial;
  G4double _absorberThickness;

  // silicon detectors, pointers since it will be a value per plane
  // the detectors will be square
  G4double* _siThickness;
  G4double* _siSide;
  // distance between planes. Calculated from absorber to Si top surfacefor plane 0, from previous pcb for the others
  // the planes should be placed in order
  G4double* _planeDist;
  
  // pcb, these values are the same for all planes
  // the pcbs are squares
  // the pcb total thickness is = pcbSubThickness + 2 * pcbMetalThickness
  G4double _pcbSubThickness; // thickness of the pcb substrate
  G4double _pcbMetalThickness; // thickness of the metal, applied on both sides
  G4double _pcbSide;

  // PCB material
  G4Material* _pcbSubMat;
  
  DetectorMessenger* _messenger;

  // variables used for construction and placement
  G4LogicalVolume* _logicWorld; // used by the functions for the placement
  G4VPhysicalVolume* _physWorld; // used to provide return value for Construct()
  G4bool _checkOverlaps; // Option to switch on/off checking of volumes overlaps

  // silicon logical volumes
  G4LogicalVolume** _logicSi;
  
  // functions to build and place the different volumes
  void ConstructWorld(); // constructs and places the world
  void ConstructSource(); // constructs and places the source and calculates the positions for PrimaryGenerationAction
  void ConstructCollimator(); // constructs and places the collimator
  void ConstructAbsorber(); // constructs and places the absorber
  void ConstructSiandPCB(G4int index); // constructs and places a silicon detector and PCB
  
  // check the inputs to the geometry and calculates an appropriate world size
  void CheckGeometry();
  
};



#endif
