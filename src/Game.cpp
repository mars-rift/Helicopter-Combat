#include "Game.h"
#include "Weapon.h"
#include "Enemy.h"
#include <iostream>

Game::Game() : helicopter("AH-1Z Viper") {
    helicopter.addWeapon(Weapon("AIM-9M Missile", 40, 60)); // Provide minDamage and maxDamage
    helicopter.addWeapon(Weapon("Precision Rocket", 20, 40)); // Provide minDamage and maxDamage
    enemies.push_back(Enemy("Drone", 50));
    enemies.push_back(Enemy("Tank", 100));
}

void Game::start() {
    int choice;
    do {
        showMenu();
        std::cin >> choice;
        handleInput(choice);
    } while (choice != 0);
}

void Game::showMenu() {
    std::cout << "1. Attack\n";
    std::cout << "2. Show Status\n";
    std::cout << "0. Exit\n";
    std::cout << "Enter your choice: ";
}

void Game::handleInput(int choice) {
    switch (choice) {
    case 1:
        if (!enemies.empty()) {
            helicopter.attack(enemies[0]);
            if (enemies[0].getHealth() <= 0) {
                enemies.erase(enemies.begin());
            }
        }
        else {
            std::cout << "No enemies left to attack!\n";
        }
        break;
    case 2:
        helicopter.showStatus();
        break;
    case 0:
        std::cout << "Exiting game...\n";
        break;
    default:
        std::cout << "Invalid choice. Try again.\n";
        break;
    }
}
