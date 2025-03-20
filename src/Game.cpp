#include "Game.h"
#include <iostream> // Include necessary header
#include <random> // Include random header

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
            // Select random enemy to attack
            std::random_device rd;
            std::mt19937 rng(rd());
            std::uniform_int_distribution<int> dist(0, static_cast<int>(enemies.size()) - 1);
            int targetIndex = dist(rng);
            Enemy& target = enemies[targetIndex];
            
            // Player attacks the selected enemy
            int weaponIndex = selectWeapon();
            if (weaponIndex >= 0) {
                int damage = helicopter.attackWithWeapon(target, weaponIndex);
                
                // Enemy counterattacks if still alive
                if (target.getHealth() > 0) {
                    int enemyDamage = target.attackDamage();
                    std::cout << target.getType() << " counterattacks!" << std::endl;
                    helicopter.takeDamage(enemyDamage);

                    if (!helicopter.isAlive()) {
                        std::cout << "Game Over! Your helicopter was destroyed!" << std::endl;
                        exit(0);
                    }
                } 
                else {
                    std::cout << target.getType() << " was destroyed!" << std::endl;
                    enemies.erase(enemies.begin() + targetIndex);
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

// New method to let player choose a weapon
int Game::selectWeapon() {
    std::cout << "\nSelect weapon to use:" << std::endl;
    helicopter.listWeapons();
    std::cout << "0. Back" << std::endl;
    
    int choice;
    std::cin >> choice;
    
    if (choice <= 0 || choice > helicopter.getWeaponCount()) {
        return -1;
    }
    return choice - 1;
}

void Game::showStatus() {
    std::cout << "Helicopter Status:" << std::endl;
    helicopter.showStatus();
    std::cout << "Enemies Status:" << std::endl;
    for (const auto& enemy : enemies) {
        std::cout << "Type: " << enemy.getType() << ", Health: " << enemy.getHealth() << std::endl;
    }
}

