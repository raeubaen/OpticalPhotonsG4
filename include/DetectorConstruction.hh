#ifndef DETECTORCONSTRUCTION_HH
#define DETECTORCONSTRUCTION_HH

#include <G4VUserDetectorConstruction.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
    DetectorConstruction() {}
    virtual ~DetectorConstruction() {}

    virtual G4VPhysicalVolume* Construct() override;
    virtual void ConstructSDandField() override;
};

#endif
