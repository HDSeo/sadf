#include "B1DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::B1DetectorConstruction()
: G4VUserDetectorConstruction(),
  fScoringVolume(0)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::~B1DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* B1DetectorConstruction::Construct()
{  
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
  

//////////////////////////////////////////////////////
G4double const torr = atmosphere/760.;

G4double temperature;
G4double pressure;

pressure = 1.e-7*torr; //3.e-18*pascal;
temperature = 300*kelvin;

G4Material* vacuum = new G4Material("Vacuum",
      1,  //Atom number, in this case use 1 for hydrogen
      1.008*g/mole, //Mass per Mole "Atomic Weight"
      1.2-25*g/cm3, //Density of vaccuum *Cant be Zero
       kStateGas, //kStateGas for Gas
       temperature,
       pressure);

//G4Material* anode_mat = new G4Material("anode",74,183.84*g/mole,19.3*g/cm3,kStateGas,2273*kelvin,pressure);

//G4Material* beryllium = new G4Material("Beryllium", 4., 9.012182*g/mole, 1.8480*g/cm3);

/////////////////////////////////////////////////////





  // Envelope parameters
  //
  G4double env_sizeXY = 50*cm, env_sizeZ = 30*cm;
   
  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //     
  // World
  //
  G4double world_sizeXY = 1.2*4.0*env_sizeXY;
  G4double world_sizeZ  = 1.2*4.0*env_sizeZ;
  
  G4Box* solidWorld =    
    new G4Box("World",                       //its name
       world_sizeXY, world_sizeXY, world_sizeZ);     //its size
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                         vacuum,           //its material
                        "World");            //its name
                                   
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking
                     
  //     
  // Envelope
  //  
  G4Box* solidEnv =    
    new G4Box("Envelope",                    //its name
        0.15*env_sizeXY, 0.15*env_sizeXY, 0.15*env_sizeZ); //its size
      
  G4LogicalVolume* logicEnv =                         
    new G4LogicalVolume(solidEnv,            //its solid
                        vacuum,             //its material
                        "Envelope");         //its name
               
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(),         //at (0,0,0)
                    logicEnv,                //its logical volume
                    "Envelope",              //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking

  //     
  // Shape 2
  //
  //G4Material* shape2_mat = nist->FindOrBuildMaterial("G4_BONE_COMPACT_ICRU");

  G4Material* shape2_mat = nist->FindOrBuildMaterial("G4_Mo");
  G4ThreeVector pos2 = G4ThreeVector(0.5*cm, 0*cm, 2.5*cm);

//////////////////////
G4RotationMatrix rotm = G4RotationMatrix();
rotm.rotateY(15.*deg);
//rotm.rotateY(0.*deg);

G4Transform3D transform = G4Transform3D(rotm,pos2);
//////////////////////

  // Trapezoid shape       
  G4double shape2_dxa = 5*cm, shape2_dxb = 5*cm;
  G4double shape2_dya = 5*cm, shape2_dyb = 5*cm;
  G4double shape2_dz  = 3*cm;      
  G4Trd* solidShape2 =    
    new G4Trd("Shape2",                      //its name
              0.5*shape2_dxa, 0.5*shape2_dxb, 
              0.5*shape2_dya, 0.5*shape2_dyb, 0.5*shape2_dz); //its size
                
  G4LogicalVolume* logicShape2 =                         
    new G4LogicalVolume(solidShape2,         //its solid
                        shape2_mat,          //its material
                        "Shape2");           //its name
               
  new G4PVPlacement(transform,                       //no rotation
                    logicShape2,             //its logical volume
                    "Shape2",                //its name
                    logicEnv,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking
///////////////////////////////

  //G4Material* shape3_mat = nist->FindOrBuildMaterial("G4_AIR");

  G4ThreeVector pos3 = G4ThreeVector(-10.*cm, 0*cm, 0*cm);
  // Trapezoid shape       
  G4double shape3_dxa = 1*cm, shape3_dxb = 1*cm;
  G4double shape3_dya = 90*cm, shape3_dyb = 90*cm;
  G4double shape3_dz  = 90*cm;      
  G4Trd* solidShape3 =    
    new G4Trd("Shape3",                      //its name
              0.5*shape3_dxa, 0.5*shape3_dxb, 
              0.5*shape3_dya, 0.5*shape3_dyb, 0.5*shape3_dz); //its size
                
  G4LogicalVolume* logicShape3 =                         
    new G4LogicalVolume(solidShape3,         //its solid
                        vacuum,          //its material
                        "Shape3");           //its name
               
  new G4PVPlacement(0,                       //no rotation
		    pos3,
                    logicShape3,             //its logical volume
                    "Shape3",                //its name
//                    logicEnv,                //its mother  volume
		    logicWorld,
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking
///////////////////////////////
/*
  G4ThreeVector pos4 = G4ThreeVector(-5.*cm, 0*cm, 0*cm);
  // Trapezoid shape       
  G4double shape4_dxa = 0.005*cm, shape4_dxb = 0.005*cm;
  G4double shape4_dya = 90*cm, shape4_dyb = 90*cm;
  G4double shape4_dz  = 90*cm;      
  G4Trd* solidShape4 =    
    new G4Trd("Shape4",                      //its name
              0.5*shape4_dxa, 0.5*shape4_dxb, 
              0.5*shape4_dya, 0.5*shape4_dyb, 0.5*shape4_dz); //its size
                
  G4LogicalVolume* logicShape4 =                         
    new G4LogicalVolume(solidShape4,         //its solid
                        //vacuum,
//			beryllium,          //its material
			shape2_mat,
			//shape2_mat,
                        "Shape4");           //its name
               
  new G4PVPlacement(0,                       //no rotation
		    pos4,
                    logicShape4,             //its logical volume
                    "Shape4",                //its name
//                    logicEnv,                //its mother  volume
		    logicWorld,
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking
*/
///////////////////////////////
  //
  fScoringVolume = logicShape2;

  //
  //always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
