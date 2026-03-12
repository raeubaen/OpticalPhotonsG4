#include <G4SDManager.hh>
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
#include "EnergyTimeSD.hh"
#include "EventAction.hh"

#include <G4RunManager.hh>
#include <G4UImanager.hh>
#include <G4UIExecutive.hh>
#include <G4OpticalPhysics.hh>
#include <QGSP_BERT.hh>

#include <G4VisManager.hh>
#include <G4VisExecutive.hh>

int main(int argc, char** argv)
{
    auto runManager = new G4RunManager();

    runManager->SetUserInitialization(new DetectorConstruction());

    auto physicsList = new QGSP_BERT();
    physicsList->RegisterPhysics(new G4OpticalPhysics());
    runManager->SetUserInitialization(physicsList);

    runManager->Initialize();

    // Initialize visualization
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();

    // Set EventAction
    G4SDManager* sdman = G4SDManager::GetSDMpointer();
    EnergyTimeSD* sipmSD = dynamic_cast<EnergyTimeSD*>(
        sdman->FindSensitiveDetector("sipmSi")
    );
    runManager->SetUserAction(new EventAction(sipmSD, "hits.csv"));

    runManager->SetUserAction(new PrimaryGeneratorAction());

    // UI executive
    G4UIExecutive* ui = nullptr;
    if (argc == 1) {
        ui = new G4UIExecutive(argc, argv);
    }

    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    if (argc > 1) {
        UImanager->ApplyCommand("/control/execute " + G4String(argv[1]));
    }

    if (ui) {
        UImanager->ApplyCommand("/control/execute macro/init_vis.mac");
        ui->SessionStart();
        delete ui;
    } else {
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command + fileName);
    }

    delete visManager;
    delete runManager;

    return 0;
}
