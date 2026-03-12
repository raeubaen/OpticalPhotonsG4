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

    fHits.push_back({time, energy});
    return true;
}
