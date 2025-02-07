#include "HelicopterCombat.h"
#include <iostream>

void HelicopterCombat::run() {
    int choice;
    do {
        showMainMenu();
        std::cin >> choice;
        handleUserInput(choice);
    } while (choice != 0);
}

void HelicopterCombat::showMainMenu() {
    std::cout << "1. Start Game\n";
    std::cout << "2. Show Game Status\n";
    std::cout << "0. Exit\n";
    std::cout << "Enter your choice: ";
}

void HelicopterCombat::handleUserInput(int choice) {
    switch (choice) {
    case 1:
        game.start();
        break;
    case 2:
        game.showStatus();
        break;
    case 0:
        std::cout << "Exiting game...\n";
        break;
    default:
        std::cout << "Invalid choice. Try again.\n";
        break;
    }
}
