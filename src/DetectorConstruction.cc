
#include <G4LogicalVolumeStore.hh>

#include <sstream>

#include "DetectorConstruction.hh"

#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4NistManager.hh>
#include <G4SystemOfUnits.hh>
#include <G4VisAttributes.hh>
#include <G4Box.hh>
#include <G4Orb.hh>
#include <G4SDManager.hh>
#include <G4SubtractionSolid.hh>

#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"

#include <G4GlobalMagFieldMessenger.hh>
#include "G4MultiFunctionalDetector.hh"
#include "G4PSEnergyDeposit.hh"

#include "EnergyTimeSD.hh"
#include "Geometry.hh"
#include "CONFIGURATION.hh"

using namespace std;

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  
  auto &geo = Geometry::Instance();
  CONFIGURATION conf;

  G4NistManager* nist = G4NistManager::Instance();

  //materials
    G4Material* vacuum = nist->FindOrBuildMaterial("G4_Galactic");
    G4Material* air = nist->FindOrBuildMaterial("G4_AIR");
    G4Material* mylar = nist->FindOrBuildMaterial("G4_MYLAR");
    G4Material* resin = nist->FindOrBuildMaterial("G4_PLEXIGLASS");
    G4Material* silicon = nist->FindOrBuildMaterial("G4_Si");

    G4Element* el_lead = nist->FindOrBuildElement("Pb", false);
    G4Element* el_fluorine = nist->FindOrBuildElement("F", false);
    G4Element* el_chlorine = nist->FindOrBuildElement("Cl", false);
    G4Element* el_oxygen = nist->FindOrBuildElement("O", false);
    G4Element* el_silicon = nist->FindOrBuildElement("Si", false);
    G4Element* el_hydrogen = nist->FindOrBuildElement("H", false);
    G4Element* el_carbon = nist->FindOrBuildElement("C", false);
    G4Element* el_copper = nist->FindOrBuildElement("Cu", false);
    G4Element* el_titanium = nist->FindOrBuildElement("Ti", false);
    G4Element* el_tungsten = nist->FindOrBuildElement("W", false);

    G4Material* PWO = new G4Material("PWO", 8.27 * g/cm3, 3);
    PWO-> AddElement(el_lead, 1);
    PWO-> AddElement(el_tungsten, 1);
    PWO-> AddElement(el_oxygen, 1);

    G4Material* PbF2_noopt = new G4Material("PbF2_noopt", 8.44 * g/cm3, 2);
    PbF2_noopt->AddElement(el_lead, 1);
    PbF2_noopt->AddElement(el_fluorine, 2);

    G4Material* PbF2 = new G4Material("PbF2", 8.44 * g/cm3, 2);
    PbF2->AddElement(el_lead, 1);
    PbF2->AddElement(el_fluorine, 2);
  //materials

  //optical 
    G4MaterialPropertiesTable* vacuum_prop = new G4MaterialPropertiesTable();
    vacuum_prop->AddProperty("RINDEX", {0.1 * eV, 4.1 * eV}, {1.0, 1.0}, 2);
    vacuum->SetMaterialPropertiesTable(vacuum_prop);

    G4MaterialPropertiesTable* air_prop = new G4MaterialPropertiesTable();
    air_prop->AddProperty("RINDEX", {0.1 * eV, 4.1 * eV}, {1.0, 1.0}, 2);
    air->SetMaterialPropertiesTable(air_prop);

    std::vector<G4double> resin_opticalEn = {0.1 * eV, 4.1 * eV};
    std::vector<G4double> resin_rindex = {1.55, 1.55}; 
    G4MaterialPropertiesTable* resin_prop = new G4MaterialPropertiesTable();
    resin_prop->AddProperty("RINDEX", resin_opticalEn, resin_rindex, 2);
    resin->SetMaterialPropertiesTable(resin_prop);

    std::vector<G4double> PbF2_opticalEn = {
      0.104 * eV, 0.151 * eV, 0.218 * eV, 0.314 * eV, 0.454 * eV, 0.656 * eV, 0.948 * eV, 1.370 * eV, 1.980 * eV, 2.860 * eV, 4.133 * eV
    };
    std::vector<G4double>  PbF2_rindex = {
      1.596, 1.668, 1.701, 1.717, 1.725, 1.731, 1.736, 1.744, 1.762, 1.803, 1.937
    };
    std::vector<G4double>  PbF2_zero = {
      0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.
    };
    std::vector<G4double> PbF2_abslen = {
      1.0 * m,  1.0 * m,  1.0 * m,  1.0 * m,  1.0 * m,
      1.0 * m,  1.0 * m,  1.0 * m,  1.0 * m,  1.0 * m,
      1.0 * m 
    };

    const G4int PbF2_numEntries = sizeof(PbF2_opticalEn) / sizeof(G4double);
    G4MaterialPropertiesTable* PbF2_prop = new G4MaterialPropertiesTable();
    PbF2_prop->AddProperty("RINDEX", PbF2_opticalEn, PbF2_rindex, PbF2_numEntries);
    PbF2_prop->AddProperty("ABSLENGTH", PbF2_opticalEn, PbF2_abslen, PbF2_numEntries);
    PbF2_prop->AddProperty("EFFICIENCY", PbF2_opticalEn, PbF2_zero, PbF2_numEntries);
    PbF2->SetMaterialPropertiesTable(PbF2_prop);
    PbF2_prop->DumpTable();
  //optical

  //surfaces
    G4OpticalSurface *wrapSurf = new G4OpticalSurface("wrapSurf");
    wrapSurf->SetType(dielectric_LUT);
    wrapSurf->SetModel(LUT); 
    // wrapSurf->SetFinish(RoughESR_LUT);
    // wrapSurf->SetFinish(polishedvm2000air);
    wrapSurf->SetFinish(polishedlumirrorair);
    // wrapSurf->SetType(dielectric_dielectric);
    // wrapSurf->SetModel(unified); 
    // wrapSurf->SetFinish(polishedfrontpainted);
    G4MaterialPropertiesTable* wrapSurf_prop = new G4MaterialPropertiesTable();
    wrapSurf_prop->AddProperty("REFLECTIVITY", {1.0*eV, 4.0*eV}, {0.995, 0.995}, 2);
    wrapSurf->SetMaterialPropertiesTable(wrapSurf_prop);
    
    G4OpticalSurface *cryGapSurf = new G4OpticalSurface("cryGapSurf");
    cryGapSurf->SetType(dielectric_dielectric);
    cryGapSurf->SetModel(unified);
    cryGapSurf->SetFinish(polished);
    G4MaterialPropertiesTable* cryGapSurf_prop = new G4MaterialPropertiesTable();
    cryGapSurf_prop->AddProperty("REFLECTIVITY", {1.0*eV, 4.0*eV}, {1.0, 1.0}, 2);
    cryGapSurf_prop->AddProperty("RINDEX", {1.0*eV, 4.0*eV}, {1.00, 1.00}, 2);
    cryGapSurf->SetMaterialPropertiesTable(cryGapSurf_prop);

    G4OpticalSurface *gapSipmSurf = new G4OpticalSurface("gapSipmSurf");
    gapSipmSurf->SetType(dielectric_dielectric);
    gapSipmSurf->SetModel(unified);
    gapSipmSurf->SetFinish(polished);
    G4MaterialPropertiesTable* gapSipmSurf_prop = new G4MaterialPropertiesTable();
    gapSipmSurf_prop->AddProperty("REFLECTIVITY", {1.0*eV, 4.0*eV}, {1.0, 1.0}, 2);
    gapSipmSurf_prop->AddProperty("RINDEX", {1.0*eV, 4.0*eV}, {1.55, 1.55}, 2);
    gapSipmSurf->SetMaterialPropertiesTable(gapSipmSurf_prop);

    G4OpticalSurface *crySipmSurf = new G4OpticalSurface("crySipmSurf");
    crySipmSurf->SetType(dielectric_dielectric);
    crySipmSurf->SetModel(unified);
    crySipmSurf->SetFinish(polished);
    G4MaterialPropertiesTable* crySipmSurf_prop = new G4MaterialPropertiesTable();
    crySipmSurf_prop->AddProperty("REFLECTIVITY", {1.0*eV, 4.0*eV}, {1.0, 1.0}, 2);
    crySipmSurf_prop->AddProperty("RINDEX", {1.0*eV, 4.0*eV}, {1.55, 1.55}, 2);
    crySipmSurf->SetMaterialPropertiesTable(crySipmSurf_prop);

    G4OpticalSurface *sipmSiSurf = new G4OpticalSurface("sipmSiSurf");
    sipmSiSurf->SetType(dielectric_metal);
    sipmSiSurf->SetModel(unified);
    sipmSiSurf->SetFinish(polished);
    G4MaterialPropertiesTable* sipmSiSurf_prop = new G4MaterialPropertiesTable();
    sipmSiSurf_prop->AddProperty("REFLECTIVITY", {1.0*eV, 4.0*eV}, {0.95, 0.95}, 2); //check
    sipmSiSurf_prop->AddProperty("EFFICIENCY", {1.0*eV, 4.0*eV}, {1.0, 1.0}, 2);
    sipmSiSurf->SetMaterialPropertiesTable(sipmSiSurf_prop);
  //surfaces

  //volumes
    G4VSolid* worldBox = new G4Box("world", geo.worldSizeX / 2, geo.worldSizeY / 2, geo.worldSizeZ / 2);
    G4LogicalVolume* worldLog = new G4LogicalVolume(worldBox, vacuum, "world");
    G4VisAttributes* invisVis = new G4VisAttributes();
    invisVis->SetVisibility(false);
    worldLog->SetVisAttributes(invisVis);

    G4VSolid* crySolid = new G4Box("cry", geo.cryL / 2, geo.cryW / 2, geo.cryW / 2);
    G4LogicalVolume* cryLog = new G4LogicalVolume(crySolid, conf.set_cryOption == 0 ? PbF2 : PWO, "cry");
    G4VisAttributes* cryVis = new G4VisAttributes(G4Colour(1, 0, 1, 0.4));
    cryVis->SetVisibility(true);
    cryVis->SetForceSolid(false);
    cryLog->SetVisAttributes(cryVis);

    // Thickness of the lateral wrap (along Y and Z)
    G4double wrapThickness = geo.wrapW;  // or any desired thickness

    // Base box that fully encloses the crystal + lateral wrap
    G4VSolid* _wrapSolid = new G4Box(
        "_wrap",
        0.5 * geo.cryL - 2*CLHEP::mm,                        // X half-length same as crystal, will subtract crystal fully
        0.5 * geo.cryW + wrapThickness,        // Y half-length: crystal + wrap thickness
        0.5 * geo.cryW + wrapThickness         // Z half-length: crystal + wrap thickness
    );

    // Subtract the crystal to leave only lateral wrap slabs
    G4VSolid* wrapSolid = new G4SubtractionSolid(
        "wrap",
        _wrapSolid,
        crySolid,              // crystal solid
        nullptr,               // no rotation
        G4ThreeVector(0, 0, 0) // aligned centers
    );

    // Logical volume
    G4LogicalVolume* wrapLog = new G4LogicalVolume(wrapSolid, mylar, "wrap");

    // Visualization attributes
    G4VisAttributes* wrapVis = new G4VisAttributes(G4Colour(1, 1, 1, 0.4));
    wrapVis->SetVisibility(true);
    wrapVis->SetForceSolid(true);
    wrapLog->SetVisAttributes(wrapVis);

    G4VPhysicalVolume* airgapPhys = nullptr;
    if (geo.sipmAirGap > 0.0) {
      G4VSolid* airgapSolid = new G4Box("airgap", geo.sipmAirGap / 2, geo.sipmPackageW, geo.sipmPackageW);
      G4LogicalVolume* airgapLog = new G4LogicalVolume(airgapSolid, air, "airgap");
      G4VisAttributes* airgapVis = new G4VisAttributes(G4Colour(0, 1, 0.5, 0.4));
      airgapVis->SetVisibility(true);
      airgapVis->SetForceSolid(true);
      airgapLog->SetVisAttributes(airgapVis);
      airgapPhys = new G4PVPlacement(nullptr, G4ThreeVector(0.5*(geo.cryL+geo.sipmAirGap), 0, 0), airgapLog, "airgap", worldLog, 0, 0);
      airgapPhys = new G4PVPlacement(nullptr, G4ThreeVector(-0.5*(geo.cryL+geo.sipmAirGap), 0, 0), airgapLog, "airgap", worldLog, 0, 0);
    }

    G4VSolid* sipmPkgSolid = new G4Box("sipmPkg", geo.sipmPackageL / 2, geo.sipmPackageW / 2, geo.sipmPackageW / 2);
    G4LogicalVolume* sipmPkgLog = new G4LogicalVolume(sipmPkgSolid, resin, "sipmPkg");
    G4VisAttributes* sipmPkgVis = new G4VisAttributes(G4Colour(1, 1, 0, 0.8));
    sipmPkgVis->SetVisibility(true);
    sipmPkgVis->SetForceSolid(true);
    sipmPkgVis->SetForceWireframe(true);
    sipmPkgLog->SetVisAttributes(sipmPkgVis);

    G4VSolid* sipmSiSolid = new G4Box("sipmSi", geo.sipmSiThick / 2, geo.sipmSiL / 2, geo.sipmSiL / 2);
    G4LogicalVolume* sipmSiLog = new G4LogicalVolume(sipmSiSolid, silicon, "sipmSi");
    G4VisAttributes* sipmSiVis = new G4VisAttributes(G4Colour(0, 1, 1, 0.4));
    sipmSiVis->SetVisibility(true);
    sipmSiVis->SetForceSolid(true);
    sipmSiLog->SetVisAttributes(sipmSiVis);

    if (conf.set_usePreshower){    
      G4double preL = conf.set_usePreshower * geo.cryL;
      G4VSolid* preSolid = new G4Box("pre", 0.5*preL, 2*geo.cryW, 2*geo.cryW);
      G4LogicalVolume* preLog = new G4LogicalVolume(preSolid, conf.set_cryOption == 0 ? PbF2_noopt : PbF2_noopt, "pre");
      G4VisAttributes* preVis = new G4VisAttributes(G4Colour(0, 1, 1, 0.4));
      preVis->SetVisibility(true);
      preVis->SetForceSolid(false);
      preLog->SetVisAttributes(preVis);
      new G4PVPlacement(nullptr, G4ThreeVector(geo.caloPosX - 0.5*(geo.cryL+preL) - 1*mm, 0, 0), preLog, "pre", worldLog, 0, 0);
    }
  //volumes

  //surfaces
    new G4LogicalSkinSurface("skin_sipmSi", sipmSiLog, sipmSiSurf);
  //surfaces

  //placements
  
    G4VPhysicalVolume* worldPhys = new G4PVPlacement(nullptr, {}, worldLog, "world", nullptr, false, 0);

    G4VPhysicalVolume* wrapPhys = new G4PVPlacement(nullptr, G4ThreeVector(-0.5*geo.wrapW+geo.caloPosX, 0, 0), wrapLog, "wrap", worldLog, 0, 0);
    G4VPhysicalVolume* cryPhys = new G4PVPlacement(nullptr, G4ThreeVector(geo.caloPosX, 0, 0), cryLog, "cry", worldLog, 0, 0);

    G4VPhysicalVolume* sipmSiPhys = new G4PVPlacement(nullptr, G4ThreeVector(geo.sipmSiSurfDist-0.5*(geo.sipmPackageL-geo.sipmSiThick), 0, 0), sipmSiLog, "sipmSi", sipmPkgLog, 0, 0); 

    // G4VPhysicalVolume* sipmSiPhys[4];
    G4VPhysicalVolume* sipmPkgPhys[8];

    /*for (int i = 0; i<8; i++) {
      G4double x = geo.sipmPos * (2*(i%2)-1);
      G4double xx = geo.sipmPos * (2*((i%4)>1)-1);
      // ostringstream name, nameSi; 
      // name<<"sipmPkg_"<<i;
      // nameSi<<"sipmSi_"<<i;
      double xPkg = geo.caloPosX + (2*(i/4)-1)*0.5*(geo.cryL+geo.sipmPackageL)+geo.sipmAirGap);
      // double xSi = geo.sipmSiSurfDist-0.5*(geo.sipmPackageL-geo.sipmSiThick);
      // sipmSiPhys[i] = new G4PVPlacement(nullptr, G4ThreeVector(xSi+xPkg, x, xx), sipmSiLog, nameSi.str(), worldLog, 0, i); 
      // sipmSiPhys[i]->SetMotherLogical(sipmPkgLog);
      sipmPkgPhys[i] = new G4PVPlacement(nullptr, G4ThreeVector(xPkg, x, xx), sipmPkgLog, "sipmPkg", worldLog, 0, i);
    }*/

    for (int i = 0; i<8; i++) {
        double y = geo.sipmPos * (2*(i%2)-1);
        double z = geo.sipmPos * (2*((i%4)>1)-1);

        int side = 2*(i/4) - 1;  // -1 for i=0..3, +1 for i=4..7

        double distance = 0.5*(geo.cryL + geo.sipmPackageL) + geo.sipmAirGap;
        double xPkg = side * distance;


	// Determine rotation for the SiPM inside the package
        G4RotationMatrix* rot = (side == -1) ? new G4RotationMatrix() : nullptr;
        if(side == -1) {
            // Rotate 180 deg around Y to face the crystal
            rot->rotateY(180*deg);
        }

        // Place the SiPM package in the world
        sipmPkgPhys[i] = new G4PVPlacement(
            rot,
            G4ThreeVector(xPkg, y, z),
            sipmPkgLog,
            "sipmPkg",
            worldLog,
            false,
            i
        );


     }
  //placements

  //surfaces
    if(conf.set_opticalOn) {

      G4LogicalBorderSurface *bord_wrap = new G4LogicalBorderSurface("cry_back", cryPhys, worldPhys, wrapSurf);
      
      if(conf.set_wrappingOn) {
        // G4LogicalSkinSurface *skin_wrap = new G4LogicalSkinSurface("skin_wrap", wrapLog, wrapSurf);
        G4LogicalBorderSurface *bord_wrap = new G4LogicalBorderSurface("bord_wrap", cryPhys, wrapPhys, wrapSurf);
      }

      for (int i = 0; i<8; i++) { 

        // new G4LogicalBorderSurface("bord_sipmsi_" + std::to_string(i), sipmPkgPhys[i], sipmSiPhys[i], sipmSiSurf); 

        if (geo.sipmAirGap > 0.0) {
          new G4LogicalBorderSurface("bord_crygap_" + std::to_string(i), cryPhys, airgapPhys, cryGapSurf);
          new G4LogicalBorderSurface("bord_gapsipm_" + std::to_string(i), airgapPhys, sipmPkgPhys[i], gapSipmSurf);
        } else {
          new G4LogicalBorderSurface("bord_crysipm_" + std::to_string(i), cryPhys, sipmPkgPhys[i], crySipmSurf);
        }
      }
    }
  //surfaces

  return worldPhys;

}

void DetectorConstruction::ConstructSDandField()
{
    CONFIGURATION conf;
    if(!conf.set_etSdOn) return;

    // Creo la SD
    auto* sipmDet = new EnergyTimeSD("sipmSi");

    // Registro la SD nel manager
    G4SDManager::GetSDMpointer()->AddNewDetector(sipmDet);

    // Associo la SD al logical volume "sipmSiLog"
    G4LogicalVolume* sipmLog = G4LogicalVolumeStore::GetInstance()->GetVolume("sipmSi");
    if(sipmLog) {
        sipmLog->SetSensitiveDetector(sipmDet);
    } else {
        G4cerr << "Error: sipmSiLog not found!" << G4endl;
    }
}

