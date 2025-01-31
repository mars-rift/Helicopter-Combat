#include "Helicopter.h"
#include <iostream>

Helicopter::Helicopter(const std::string& name) : name(name), health(100) {}

void Helicopter::attack(Enemy& target) {
    if (weapons.empty()) {
        std::cout << name << " has no weapons to attack " << target.getType() << "!" << std::endl;
        return;
    }

    for (const auto& weapon : weapons) {
        int damage = weapon.getDamage();
        std::cout << name << " attacks " << target.getType() << " with " << weapon.getName() << " causing " << damage << " damage." << std::endl;
        target.takeDamage(damage);
        damageTable.push_back({ weapon.getName(), damage });
    }
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

void Helicopter::printDamageTable() const {
    std::cout << "Damage Table:" << std::endl;
    for (const auto& entry : damageTable) {
        std::cout << "Weapon: " << entry.first << ", Damage: " << entry.second << std::endl;
    }
}
