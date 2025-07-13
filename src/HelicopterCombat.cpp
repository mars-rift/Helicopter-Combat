#include "HelicopterCombat.h"
#include <iostream>

void HelicopterCombat::run() {
    std::cout << "\n=== ADVANCED HELICOPTER COMBAT SIMULATOR ===" << std::endl;
    //std::cout <<        "" << std::endl;
    
    int choice;
    do {
        showMainMenu();
        std::cin >> choice;
        handleUserInput(choice);
    } while (choice != 0);
}

void HelicopterCombat::showMainMenu() {
    std::cout << "\n=== MAIN MENU ===" << std::endl;
    std::cout << "1. Start Mission" << std::endl;
    std::cout << "2. Free Flight Mode" << std::endl;
    std::cout << "3. Training Simulator" << std::endl;
    std::cout << "4. Mission Briefing Room" << std::endl;
    std::cout << "5. Aircraft Status" << std::endl;
    std::cout << "6. Environmental Conditions" << std::endl;
    std::cout << "7. Simulation Settings" << std::endl;
    std::cout << "0. Exit Simulator" << std::endl;
    std::cout << "Enter your choice: ";
}

void HelicopterCombat::handleUserInput(int choice) {
    switch (choice) {
    case 1:
        std::cout << "\n>> Starting Mission Mode..." << std::endl;
        game.showMissionMenu();
        break;
    case 2:
        std::cout << "\n>> Entering Free Flight Mode..." << std::endl;
        game.enterFlightMode();
        break;
    case 3:
        std::cout << "\n>> Launching Training Simulator..." << std::endl;
        game.startMission(static_cast<MissionType>(0)); // Training mission
        break;
    case 4:
        std::cout << "\n>> Accessing Mission Briefing Room..." << std::endl;
        game.showMissionBriefing();
        break;
    case 5:
        std::cout << "\n>> Aircraft Systems Status:" << std::endl;
        game.showSystemStatus();
        break;
    case 6:
        std::cout << "\n>> Environmental Conditions:" << std::endl;
        game.showEnvironmentalStatus();
        break;
    case 7:
        std::cout << "\n>> Simulation Settings:" << std::endl;
        game.showSimulationControls();
        break;
    case 0:
        std::cout << "\n>> Shutting down simulator..." << std::endl;
        std::cout << "Thank you for flying! Stay safe out there, pilot." << std::endl;
        break;
    default:
        std::cout << "[!] Invalid choice. Please select a valid option." << std::endl;
        break;
    }
}
