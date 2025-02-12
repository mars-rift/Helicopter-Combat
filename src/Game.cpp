#include "Game.h"
#include <iostream> // Include necessary header

// This is the Game constructor implementation
Game::Game() : helicopter("AH-1Z Viper") {
    // Add different weapons
    helicopter.addWeapon(Weapon("AIM-9M Missile", 40, 60));
    helicopter.addWeapon(Weapon("Precision Rocket", 20, 40));
    helicopter.addWeapon(Weapon("Machine Gun", 10, 25));    // New weapon
    helicopter.addWeapon(Weapon("Air-to-Ground Missile", 30, 50));  // New weapon

    // Create enemies
    enemies.push_back(Enemy("Scout Drone", 30, 5, 15));     // Fast, weak enemy
    enemies.push_back(Enemy("Attack Drone", 50, 10, 20));   // Standard drone
    enemies.push_back(Enemy("Light Tank", 75, 15, 25));     // Medium enemy
    enemies.push_back(Enemy("Heavy Tank", 100, 20, 35));    // Strong enemy
    enemies.push_back(Enemy("SAM Site", 60, 25, 40));      // High damage, low health
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
            // Player attacks a random enemy
            helicopter.attackRandomEnemy(enemies);

            // Enemy counterattacks if still alive
            if (!enemies.empty() && enemies[0].getHealth() > 0) {
                int enemyDamage = enemies[0].attackDamage();
                std::cout << enemies[0].getType() << " counterattacks!" << std::endl;
                helicopter.takeDamage(enemyDamage);

                if (!helicopter.isAlive()) {
                    std::cout << "Game Over! Your helicopter was destroyed!" << std::endl;
                    exit(0);
                }
            }
        }
        else {
            std::cout << "No enemies left to attack!\n";
        }
        break;
    case 2:
        showStatus();
        break;
    case 0:
        std::cout << "Exiting game...\n";
        break;
    default:
        std::cout << "Invalid choice. Try again.\n";
        break;
    }
}

void Game::showStatus() {
    std::cout << "Helicopter Status:" << std::endl;
    helicopter.showStatus();
    std::cout << "Enemies Status:" << std::endl;
    for (const auto& enemy : enemies) {
        std::cout << "Type: " << enemy.getType() << ", Health: " << enemy.getHealth() << std::endl;
    }
}

