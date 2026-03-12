#include "PrimaryGeneratorMessenger.hh"
#include "PrimaryGeneratorAction.hh"

#include "G4UIcmdWithADouble.hh"
#include "G4UIdirectory.hh"

PrimaryGeneratorMessenger::PrimaryGeneratorMessenger(PrimaryGeneratorAction* action)
: fAction(action)
{
}

PrimaryGeneratorMessenger::~PrimaryGeneratorMessenger()
{
}

