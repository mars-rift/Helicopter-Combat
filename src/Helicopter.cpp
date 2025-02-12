#include "Helicopter.h"
#include <iostream>

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
