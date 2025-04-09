#include "Game.h"
#include <iostream>  // Include necessary header
#include <random>    // Include random header
#include <limits>    // For std::numeric_limits
#include <cstdlib>   // For exit()

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
    try {
        int choice;
        do {
            showMenu();
            std::cin >> choice;
            handleInput(choice);
        } while (choice != 0);
    } catch (const std::exception& e) {
        std::cerr << "Error occurred: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown error occurred" << std::endl;
    }
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
            // Let player select enemy to attack
            int targetIndex = selectEnemy();
            if (targetIndex >= 0) {
                Enemy& target = enemies[targetIndex];
                
                // Player attacks the selected enemy
                int weaponIndex = selectWeapon();
                if (weaponIndex >= 0) {
                    helicopter.attackWithWeapon(target, weaponIndex);
                    
                    // Enemy counterattacks if still alive
                    if (target.getHealth() > 0) {
                        int enemyDamage = target.attackDamage();
                        std::cout << target.getType() << " counterattacks!" << std::endl;
                        helicopter.takeDamage(enemyDamage);

                        if (!helicopter.isAlive()) {
                            std::cout << "Game Over! Your helicopter was destroyed!" << std::endl;
                            showStatus();
                            std::cout << "MISSION FAILED" << std::endl;
                            exit(0);
                        }
                    } 
                    else {
                        std::cout << target.getType() << " was destroyed!" << std::endl;
                        enemies.erase(enemies.begin() + targetIndex);
                        
                        // Check if all enemies are defeated
                        if (enemies.empty()) {
                            std::cout << "\n=== VICTORY! All enemies have been defeated! ===\n" << std::endl;
                            std::cout << "Final Status:" << std::endl;
                            showStatus();
                            std::cout << "\nMISSION COMPLETE!" << std::endl;
                            exit(0);
                        }
                    }
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
    while (true) {
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear(); // Clear error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number between 0 and " 
                      << helicopter.getWeaponCount() << ": ";
        } else if (choice < 0 || choice > helicopter.getWeaponCount()) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid choice. Please enter a number between 0 and " 
                      << helicopter.getWeaponCount() << ": ";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        }
    }

    const int BACK_OPTION = 0;
    if (choice == BACK_OPTION) {
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

// New method to select enemy target
int Game::selectEnemy() {
    std::cout << "\nSelect enemy to attack:" << std::endl;
    for (size_t i = 0; i < enemies.size(); i++) {
        std::cout << (i+1) << ". " << enemies[i].getType() 
                  << " (Health: " << enemies[i].getHealth() << ")" << std::endl;
    }
    std::cout << "0. Back" << std::endl;
    
    int choice;
    while (true) {
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear(); // Clear error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number between 0 and " 
                      << enemies.size() << ": ";
        } else if (choice < 0 || choice > static_cast<int>(enemies.size())) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid choice. Please enter a number between 0 and " 
                      << enemies.size() << ": ";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        }
    }
    
    const int BACK_OPTION = 0;
    if (choice == BACK_OPTION) {
        return -1;
    }
    return choice - 1;
}

