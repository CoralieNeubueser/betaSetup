// modified from example B1

#include "DetectorConstruction.hh"
#include "SiDetSD.hh"
#include "DetectorMessenger.hh"

#include "G4PhysicalConstants.hh"
#include "G4SubtractionSolid.hh"
#include "G4StateManager.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"

#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4ProductionCuts.hh"

#include "G4UserLimits.hh"

int DetectorConstruction::nPlanes = 3; // static member for number of planes (Si+PCB)

DetectorConstruction::DetectorConstruction()
  : G4VUserDetectorConstruction()
{
  //_worldSide = 45*cm; // maximum space in climate chamber, the correct one is calculated in Construct()
  _checkOverlaps = true;  

  _siThickness = new G4double[nPlanes];
  _siSide = new G4double[nPlanes];
  _planeDist = new G4double[nPlanes];

  // give default values to geometry
  _sourceCaseHeight = 46 * mm; // do not change!
  _sourceCaseDiam = 60 * mm; // do not change!

  _collThickness = 10 * mm;
  _collHoleDiam = 2 * mm;

  _absorberMaterial = "G4_POLYETHYLENE";
  _absorberThickness = 1 * mm;

  for(G4int i = 0; i < nPlanes; ++i){
    _siThickness[i] = 100 * um;
    _siSide[i] = 1 * cm;
    _planeDist[i] = 1 * cm;
  }
  
  _pcbSubThickness = 0.8 * mm;
  _pcbMetalThickness = 35 * um;
  _pcbSide = 5 * cm;

  // material definition here so that it takes less time recalculating geometry
  // define the PCB substrate material, the pcbs used in the lab have
  // rogers materials RO4350B
  // density 1.86 g/cm3
  // it should be fiber glass in plastic or ceramics
  //glass and bakelite are used in proportions that should give the right density
  _pcbSubMat =
    new G4Material("pcbSubMat",  // name
		   1.86 * g/cm3, // density
		   2);           // number of components
  // the fractional masses are calculated in order to obtain the desired density, hopefully in the correct way
  // fractional mass1 = m1 / M = (rho1 * (rho2 - rho)) / (rho * (rho2 - rho1))
  // M = m1 + m2
  // bakelite density: 1.25 g/cm3
  _pcbSubMat->AddMaterial(G4NistManager::Instance()->FindOrBuildMaterial("G4_BAKELITE"), // material
			 0.25);                                                         // fractional mass
  // pyrex glass density: 2.23 g/cm3
  _pcbSubMat->AddMaterial(G4NistManager::Instance()->FindOrBuildMaterial("G4_Pyrex_Glass"), // material
			 0.75);                                                            // fractional mass

  _messenger = new DetectorMessenger(this);

  _logicSi = new G4LogicalVolume*[nPlanes];

  // _siRegion = 0;
  
  return;
}

DetectorConstruction::~DetectorConstruction()
{
  return;
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{  
  // Clean old geometry, if any. from AnaEx01
  //
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  // if(_siRegion) // delete region in case it exists
  //   delete _siRegion;
  // // create region
  // _siRegion = new G4Region("SiliconRegion");
  // G4ProductionCuts* cuts = new G4ProductionCuts(); // production cuts must be set for the region
  // _siRegion->SetProductionCuts(cuts);
    
  // check input geometry and adjust world size to contain setup
  CheckGeometry();
  
  // World
  ConstructWorld();

  // radioactive source
  ConstructSource();

  // collimator
  if(_collThickness != 0)
    ConstructCollimator();

  // absorber
  if(_absorberThickness != 0)
    ConstructAbsorber();

  // silicon and pcb planes
  for(G4int i = 0; i < nPlanes; ++i)
    ConstructSiandPCB(i);
    
  //
  //always return the physical World
  //
  return _physWorld;
}

void DetectorConstruction::ConstructWorld()
{
  // the word will be a cube with side _worldSide
  G4Material* world_mat = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
  
  G4Box* solidWorld =    
    new G4Box("World",                       //its name
       0.5*_worldSide, 0.5*_worldSide, 0.5*_worldSide);     //its size
      
  _logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name
                                   
  _physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      _logicWorld,           //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      _checkOverlaps);       //overlaps checking
  return;
}

void DetectorConstruction::ConstructSource()
{
  // source case

  // total envelope of plexiglass shielding
  // G4double sourceDiameter = 60*mm;
  // G4double sourceHeight = 46*mm;
  G4double sourceDiameter = _sourceCaseDiam;
  G4double sourceHeight = _sourceCaseHeight;

  // total envelope of the metal plug containing the source
  G4double metalPlugDiameter = 12*mm;
  G4double metalPlugLength = 50*mm;

  // distance between the center of the plexyglass cylinder (in height) and the center of the plug (axis)
  G4double plugDisplacement = 7*mm;
  
  // cylinder used to provide an exit path for the particles
  G4double exitHoleDiameter = 2*mm;
  // the length takes into account that the plexiglass has to be removed also in the part where the metal plug is placed,
  // as the metal plug hole should not be filled with the casing material
  G4double exitHoleLength = 2 * (sourceHeight / 2 - plugDisplacement + metalPlugDiameter / 2);
  
  // outermost envelope of the source
  G4Tubs* outerCylinder =
    new G4Tubs("outerCylinder",      // name
	       0,                    // inner radius
	       0.5 * sourceDiameter, // outer radius
	       0.5 * sourceHeight,   // half height
	       0,                    // starting angle
	       2*pi*rad);            // finish angle

  // hole to allow particle to exit
  G4Tubs* exitHole = 
    new G4Tubs("exitHole",             // name
	       0,                      // inner radius
	       0.5 * exitHoleDiameter, // outer radius
	       0.5 * exitHoleLength,   // half height
	       0,                      // starting angle
	       2*pi*rad);              // finish angle
  
  
  // subtract the exit hole from the souce housing
  G4RotationMatrix* rot0 = new G4RotationMatrix; // no rotation
  G4ThreeVector trans0(0, 0, 0.5 * sourceHeight); // translation of the exit hole
  G4SubtractionSolid* sourceHousingShape = new G4SubtractionSolid("sourceHousingShape", outerCylinder, exitHole, rot0, trans0); // apply exit hole
  
  G4Material* sourceHousing_mat = G4NistManager::Instance()->FindOrBuildMaterial("G4_PLEXIGLASS");
  
  G4LogicalVolume* logicSourceHousing =
    new G4LogicalVolume(sourceHousingShape,  // solid
			sourceHousing_mat,   // material
			"sourceHousing_LV"); // name

  // metal cylinder containing the source within the housing
  G4RotationMatrix* rot1 = new G4RotationMatrix; // rotation of the plug hole, do not modify after building the subtraction solid!
  rot1->rotateX(pi/2 * rad);
  G4ThreeVector trans1(0, 0, 0); // translation of the plug hole
  G4Tubs* plugFull = 
    new G4Tubs("plugFull",              // name
	       0,                       // inner radius
	       0.5 * metalPlugDiameter, // outer radius
	       0.5 * metalPlugLength,   // half height
	       0,                       // starting angle
	       2*pi*rad);               // finish angle

  G4SubtractionSolid* plugShape = new G4SubtractionSolid("plugShape", plugFull, exitHole, rot1, trans1); // hole in the metal plug

  G4Material* plug_mat = G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");

  G4LogicalVolume* logicPlug = 
    new G4LogicalVolume(plugShape,   // solid
			plug_mat,    // material
			"plug_LV");  // name

  // place the metal plug in the source housing
  new G4PVPlacement(rot1,                                   // rotation, can reuse this one....
		    G4ThreeVector(0,0,plugDisplacement),    // position
		    logicPlug,                              // logical volume
		    "plug_PV",                              // name
		    logicSourceHousing,                     // mother volume
		    false,                                  // boolean operation
		    0,                                      // copy number
		    _checkOverlaps);                        // overlap checking
 

  // place source housing in the logicworld, the source is placed against the world side at -z
  new G4PVPlacement(0,                                                      // rotation
		    G4ThreeVector(0,0, -0.5 *(_worldSide - sourceHeight)),  // position
		    logicSourceHousing,                                     // logical volume
		    "sourceHousing_PV",                                     // name
		    _logicWorld,                                            // mother volume
		    false,                                                  // boolean operation
		    0,                                                      // copy number
		    _checkOverlaps);                                        // overlap checking

  // position of the radioactive material and its radius (thought as a sphere)
  _sourcePosZ = -0.5 *(_worldSide - sourceHeight - 2 * plugDisplacement);
  _sourceRadius = 0.5*exitHoleDiameter;
  
  return;
}

void DetectorConstruction::ConstructCollimator()
{
  G4Tubs* solidCollimator =
    new G4Tubs("solidCollimator",     // name
	       0.5 * _collHoleDiam,   // inner radius
	       0.5 * _sourceCaseDiam, // outer radius
	       0.5 * _collThickness,  // half height
	       0,                     // starting angle
	       2*pi*rad);             // finish angle

  G4Material* collMat = G4NistManager::Instance()->FindOrBuildMaterial("G4_BRASS");

  G4LogicalVolume* logicCollimator =
    new G4LogicalVolume(solidCollimator,  // solid
  			collMat,          // material
  			"collimator_LV"); // name

  G4double zpos = -0.5 * _worldSide;
  zpos += _sourceCaseHeight + _absorberThickness + 0.5 * _collThickness;

  new G4PVPlacement(0,                                // rotation
  		    G4ThreeVector(0,0,zpos),          // position
  		    logicCollimator,                  // logical volume
  		    "collimator_PV",                  // name
  		    _logicWorld,                      // mother volume
  		    false,                            // boolean operation
  		    0,                                // copy number
  		    _checkOverlaps);                  // overlap checking
  
  return;
}

void DetectorConstruction::ConstructAbsorber()
{
  G4Tubs* solidAbsorber =
    new G4Tubs("solidAbsorber",          // name
	       0,                        // inner radius
	       0.5 * _sourceCaseDiam,    // outer radius
	       0.5 * _absorberThickness, // half height
	       0,                        // starting angle
	       2*pi*rad);                // finish angle

  G4Material* absMat = G4NistManager::Instance()->FindOrBuildMaterial(_absorberMaterial);

  G4LogicalVolume* logicAbsorber =
    new G4LogicalVolume(solidAbsorber,  // solid
  			absMat,         // material
  			"absorber_LV"); // name


  G4double zpos = -0.5 * _worldSide;
  zpos += _sourceCaseHeight + 0.5 * _absorberThickness;

  new G4PVPlacement(0,                                // rotation
  		    G4ThreeVector(0,0,zpos),          // position
  		    logicAbsorber,                    // logical volume
  		    "absorber_PV",                    // name
  		    _logicWorld,                      // mother volume
  		    false,                            // boolean operation
  		    0,                                // copy number
  		    _checkOverlaps);                  // overlap checking
  
  return;
}

void DetectorConstruction::ConstructSiandPCB(G4int index)
{
  if(index < 0 || index >= nPlanes){
    G4cout << "DetectorConstruction::ConstructSiandPCB Invalid index!" << G4endl;
    return;
  }

  //
  // silicon
  //
  
  G4Material* siMat = G4NistManager::Instance()->FindOrBuildMaterial("G4_Si");

  G4String name = "si";
  name += std::to_string(index);
  
  G4Box* solidSi =
    new G4Box(name,                                                                    // name
  	      0.5 * _siSide[index],  0.5 * _siSide[index], 0.5 * _siThickness[index]); // size

  name += "_LV";
  
  _logicSi[index] =
    new G4LogicalVolume(solidSi, // solid
  			siMat,   // material
  			name);   // name

  // limit step length in the detector
  G4UserLimits* userLimits = new G4UserLimits(1*um);
  _logicSi[index]->SetUserLimits(userLimits);


  // add logic volume to the region
  // _logicSi[index]->SetRegion(_siRegion);
  // _siRegion->AddRootLogicalVolume(_logicSi[index]);
  
  name = "si";
  name += std::to_string(index);
  name += "_PV";

  // position along z
  G4double zpos = -0.5 * _worldSide;
  zpos += _sourceCaseHeight + _collThickness + _absorberThickness;
  zpos += _planeDist[0];
  if(index > 0){
    G4int i = 1;
    do{
      zpos += _pcbSubThickness + 2 * +_pcbMetalThickness + _planeDist[i]; // add pcb thickness and plane distance
      zpos += _siThickness[i - 1]; // add the thickness of silicon sensor in plane in front
      ++i;
    }while(i <= index);
  }
  zpos += 0.5 * _siThickness[index];
    
  new G4PVPlacement(0,                                // rotation
  		    G4ThreeVector(0,0,zpos),          // position
  		    _logicSi[index],                  // logical volume
  		    name,                             // name
  		    _logicWorld,                      // mother volume
  		    false,                            // boolean operation
  		    0,                                // copy number
  		    _checkOverlaps);                  // overlap checking

  // avoid building pcbs if their thickness is 0
  if(_pcbSubThickness == 0 || _pcbMetalThickness == 0)
    return;
  
  //
  // PCB
  //

  // pcb material defined in constructor
  
  G4Material* pcbMetal = G4NistManager::Instance()->FindOrBuildMaterial("G4_Cu");

  // create box to contain both substrate and metal
  name = "pcb";
  name += std::to_string(index);

  G4Box* solidPCB =
    new G4Box(name,
	      0.5 * _pcbSide, 0.5 * _pcbSide, 0.5 * (_pcbSubThickness + 2 * _pcbMetalThickness)); // should contain 2 metal layers

  name += "_LV";
  G4LogicalVolume* logicPCB =
    new G4LogicalVolume(solidPCB,
			_pcbSubMat,
			name);

  // create the metal layers
  name = "pcbMetal";
  G4Box* solidMetal =
    new G4Box(name,
	      0.5 * _pcbSide, 0.5 * _pcbSide, 0.5 * _pcbMetalThickness);
  
  name += "_LV";
  G4LogicalVolume* logicMetal =
    new G4LogicalVolume(solidMetal,
			pcbMetal,
			name);

  name = "pcbMetal_PV";
  new G4PVPlacement(0,                                                                   // rotation
  		    G4ThreeVector(0,0, -0.5 * (_pcbSubThickness + _pcbMetalThickness)),  // position
  		    logicMetal,                                                          // logical volume
  		    name,                                                                // name
  		    logicPCB,                                                            // mother volume
  		    false,                                                               // boolean operation
  		    0,                                                                   // copy number
  		    _checkOverlaps);                                                     // overlap checking

  new G4PVPlacement(0,                                                                   // rotation
  		    G4ThreeVector(0,0, 0.5 * (_pcbSubThickness + _pcbMetalThickness)),   // position
  		    logicMetal,                                                          // logical volume
  		    name,                                                                // name
  		    logicPCB,                                                            // mother volume
  		    false,                                                               // boolean operation
  		    1,                                                                   // copy number
  		    _checkOverlaps);                                                     // overlap checking

  zpos += 0.5 * _siThickness[index]; // add other half of si thicnkess
  zpos += 0.5 * (_pcbSubThickness + 2 * _pcbMetalThickness); // add half of pcb thickness

  name = "pcb";
  name += std::to_string(index);
  name += "_PV";
  new G4PVPlacement(0,                                // rotation
  		    G4ThreeVector(0,0,zpos),          // position
  		    logicPCB,                          // logical volume
  		    name,                             // name
  		    _logicWorld,                      // mother volume
  		    false,                            // boolean operation
  		    0,                                // copy number
  		    _checkOverlaps);                  // overlap checking

  return;
}

void DetectorConstruction::CheckGeometry()
{
  // check that the silicon sensors have non-zero thickness
  for(G4int i = 0; i < nPlanes; ++i)
    if(_siThickness[i] == 0){
      G4cout << "DetectorConstruction::CheckGeometry Thickness of silicon " << i << " is 0!!" << G4endl;
      G4cout << "Thickness set to 10 um" << G4endl;
      _siThickness[i] = 10 * um;
    }

  // check that the pcb side is >= than the silicon side for each plane
  for(G4int i = 0; i < nPlanes; ++i)
    if(_pcbSide < _siSide[i]){
      G4cout << "DetectorConstruction::CheckGeometry PCB side si smaller than Si side for plane " << i << " !!" << G4endl;
      G4cout << "PCB side set to " << _siSide[i] << " mm" << G4endl;
      _pcbSide = _siSide[i];
    }
  
  // make the world accomodate the setup
  _worldSide = _sourceCaseHeight + _collThickness + _absorberThickness;
  for(G4int i = 0; i < nPlanes; ++i)
    _worldSide += _planeDist[i] + _siThickness[i] + _pcbSubThickness + 2*_pcbMetalThickness;

  _worldSide += 1 * cm; // add 1 cm to have nicer track visualization
  
  // check if everything fits in the other world dimensions
  if(_worldSide < _sourceCaseDiam || _worldSide < _pcbSide){
    if(_sourceCaseDiam > _pcbSide)
      _worldSide = _sourceCaseDiam;
    else
      _worldSide = _pcbSide;
  }
 
  return;
}

void DetectorConstruction::ConstructSDandField()
{
  // Sensitive detectors

  G4String name;
  for(G4int i = 0; i < nPlanes; ++i){
    name = "siSensDet";
    name += std::to_string(i);
    // delete old sensitive detectors if they exist, to avoid warnings
    G4VSensitiveDetector* oldDet = G4SDManager::GetSDMpointer()->FindSensitiveDetector(name, false);
    if(oldDet)
      delete oldDet;
    
    SiDetSD* sensDet = new SiDetSD(name, i); // name must be unique
    G4SDManager::GetSDMpointer()->AddNewDetector(sensDet);
    _logicSi[i]->SetSensitiveDetector(sensDet);
  }
  return;
}

void DetectorConstruction::SetSiThickness(G4int index, G4double thickness)
{
  if(index < 0 || index >= nPlanes){
    G4cout << "DetectorConstruction::SetSiThickness Invalid index!" << G4endl;
    return;
  }
  
  _siThickness[index] = thickness;
  
  return;
}

void DetectorConstruction::SetSiSide(G4int index, G4double side)
{
  if(index < 0 || index >= nPlanes){
    G4cout << "DetectorConstruction::SetSiSide Invalid index!" << G4endl;
    return;
  }

  _siSide[index] = side;
  
  return;
}

void DetectorConstruction::SetPlaneDistance(G4int index, G4double distance)
{
  if(index < 0 || index >= nPlanes){
    G4cout << "DetectorConstruction::SetPlaneDistance Invalid index!" << G4endl;
    return;
  }

  _planeDist[index] = distance;
  
  return;
}

void DetectorConstruction::Reinitialize()
{
  if(G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit ){ // from AnaEx01
    G4RunManager::GetRunManager()->ReinitializeGeometry();
  }

  return;
}
