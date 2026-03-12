#ifndef EVENTACTION_HH
#define EVENTACTION_HH

#include <G4UserEventAction.hh>
#include "EnergyTimeSD.hh"
#include <fstream>
#include <string>

class EventAction : public G4UserEventAction
{
public:
    EventAction(EnergyTimeSD* sd, const std::string& filename="hits.csv");
    virtual ~EventAction();

    virtual void EndOfEventAction(const G4Event*) override;

private:
    EnergyTimeSD* fSD;
    std::ofstream fOut;
};

#endif
