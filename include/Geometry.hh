#ifndef GEOMETRY_HH
#define GEOMETRY_HH

#include <G4SystemOfUnits.hh>

class Geometry
{
public:
    static Geometry& Instance()
    {
        static Geometry instance;
        return instance;
    }

    // world
    G4double worldSizeX = 5.0*m;
    G4double worldSizeY = 5.0*m;
    G4double worldSizeZ = 5.0*m;

    // crystal
    G4double cryL = 20.0*cm;
    G4double cryW = 2.0*cm;

    // wrapping
    G4double wrapW = 0.2*cm;

    // SiPM
    G4double sipmAirGap = 0.1*cm;
    G4double sipmPackageL = 1.0*cm;
    G4double sipmPackageW = 1.0*cm;
    G4double sipmSiThick = 0.05*cm;
    G4double sipmSiL = 0.5*cm;
    G4double sipmPos = 0.5*cm;
    G4double sipmSiSurfDist = 0.0;

    // calorimeter
    G4double caloPosX = 0.0;

private:
    Geometry() {}
    Geometry(const Geometry&) = delete;
    Geometry& operator=(const Geometry&) = delete;
};

#endif
