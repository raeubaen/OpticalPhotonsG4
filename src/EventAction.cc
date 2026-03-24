#include "EventAction.hh"
#include <G4Event.hh>

EventAction::EventAction(EnergyTimeSD* sd, const std::string& filename)
: fSD(sd)
{
    fOut.open(filename, std::ios::out);
    fOut << "time_ns,energy_eV,x_mm,y_mm,z_mm,id\n";
}

EventAction::~EventAction()
{
    fOut.close();
}

void EventAction::EndOfEventAction(const G4Event*)
{
    const auto& hits = fSD->GetHits();
    for(const auto& h : hits) {
        fOut << h.time_ns << "," << h.energy_eV <<  "," << h.x_mm << "," << h.y_mm << "," << h.z_mm << "," << h.sipmID << "\n";
    }
    fSD->ClearHits();
}
