#ifndef PrimaryGeneratorMessenger_h
#define PrimaryGeneratorMessenger_h

#include "G4UImessenger.hh"

class PrimaryGeneratorAction;
class G4UIcmdWithADouble;

class PrimaryGeneratorMessenger : public G4UImessenger
{
public:
    PrimaryGeneratorMessenger(PrimaryGeneratorAction* action);
    ~PrimaryGeneratorMessenger();

private:
    PrimaryGeneratorAction* fAction;
};

#endif
