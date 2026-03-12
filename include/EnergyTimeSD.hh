#ifndef ENERGYTIMESD_HH
#define ENERGYTIMESD_HH

#include <G4VSensitiveDetector.hh>
#include <vector>

struct HitData {
    double time_ns;
    double energy_eV;
};

class EnergyTimeSD : public G4VSensitiveDetector
{
public:
    EnergyTimeSD(const G4String& name);
    virtual ~EnergyTimeSD() {}

    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* hist) override;

    const std::vector<HitData>& GetHits() const { return fHits; }
    void ClearHits() { fHits.clear(); }

private:
    std::vector<HitData> fHits;
};

#endif
