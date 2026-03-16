#include "EnergyTimeSD.hh"
#include <G4Step.hh>
#include <G4OpticalPhoton.hh>
#include <G4ParticleDefinition.hh>
#include <G4ios.hh>

EnergyTimeSD::EnergyTimeSD(const G4String& name)
: G4VSensitiveDetector(name)
{
}

G4bool EnergyTimeSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
    auto particle = step->GetTrack()->GetDefinition();
    if(particle != G4OpticalPhoton::OpticalPhotonDefinition()) return false;

    double time = step->GetPreStepPoint()->GetGlobalTime()/CLHEP::ns;
    double energy = step->GetPreStepPoint()->GetKineticEnergy()/CLHEP::eV;
    G4ThreeVector pos = step->GetPreStepPoint()->GetPosition();

    double x = pos.x()/CLHEP::mm;
    double y = pos.y()/CLHEP::mm;
    double z = pos.z()/CLHEP::mm;


    auto touch = step->GetPreStepPoint()->GetTouchableHandle();

    int sipmID = touch->GetCopyNumber(1); // package

    fHits.push_back({time, energy, x, y, z, sipmID});

    return true;
}
