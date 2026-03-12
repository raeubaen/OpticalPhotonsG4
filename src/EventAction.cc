#include "EventAction.hh"
#include <G4Event.hh>

EventAction::EventAction(EnergyTimeSD* sd, const std::string& filename)
: fSD(sd)
{
    fOut.open(filename, std::ios::out);
    fOut << "time_ns,energy_eV\n";
}

EventAction::~EventAction()
{
    fOut.close();
}

void EventAction::EndOfEventAction(const G4Event*)
{
    const auto& hits = fSD->GetHits();
    for(const auto& h : hits) {
        fOut << h.time_ns << "," << h.energy_eV << "\n";
    }
    fSD->ClearHits();
}
