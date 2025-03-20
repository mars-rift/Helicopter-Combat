#include "Helicopter.h"
#include <iostream>
#include <random>

Helicopter::Helicopter(const std::string& name) : name(name), health(100) {}

Helicopter::Helicopter() : name("Default Helicopter"), health(100) {}

void Helicopter::attack(Enemy& target) {
    if (weapons.empty()) {
        std::cout << name << " has no weapons to attack " << target.getType() << "!" << std::endl;
        return;
    }

    for (const auto& weapon : weapons) {
        int damage = weapon.getDamage();
        std::cout << name << " attacks " << target.getType() << " with " << weapon.getName() << " causing " << damage << " damage." << std::endl;
        target.takeDamage(damage);
    }
}

void Helicopter::attackRandomEnemy(std::vector<Enemy>& enemies) {
    if (enemies.empty()) {
        std::cout << "No enemies left to attack!\n";
        return;
    }

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> dist(0, enemies.size() - 1);
    int randomIndex = dist(rng);

    attack(enemies[randomIndex]);

    // Remove defeated enemy
    if (enemies[randomIndex].getHealth() <= 0) {
        std::cout << enemies[randomIndex].getType() << " was destroyed!" << std::endl;
        enemies.erase(enemies.begin() + randomIndex);
    }
}

void Helicopter::takeDamage(int damage) {
    health -= damage;
    if (health < 0) health = 0;
    std::cout << name << " takes " << damage << " damage! Health: " << health << std::endl;
}

bool Helicopter::isAlive() const {
    return health > 0;
}

void Helicopter::addWeapon(const Weapon& weapon) {
    weapons.push_back(weapon);
}

void Helicopter::showStatus() const {
    std::cout << "Helicopter: " << name << ", Health: " << health << ", Weapons: ";
    for (const auto& weapon : weapons) {
        std::cout << weapon.getName() << " ";
    }
    std::cout << std::endl;
}

int Helicopter::attackWithWeapon(Enemy& target, int weaponIndex) {
    if (weaponIndex < 0 || weaponIndex >= weapons.size()) {
        std::cout << "Invalid weapon selection." << std::endl;
        return 0;
    }
    
    const Weapon& weapon = weapons[weaponIndex];
    int damage = weapon.getDamage();
    std::cout << name << " attacks " << target.getType() 
              << " with " << weapon.getName() 
              << " causing " << damage << " damage." << std::endl;
    target.takeDamage(damage);
    return damage;
}

void Helicopter::listWeapons() const {
    for (size_t i = 0; i < weapons.size(); i++) {
        std::cout << (i+1) << ". " << weapons[i].getName() << std::endl;
    }
}

int Helicopter::getWeaponCount() const {
    return weapons.size();
}
