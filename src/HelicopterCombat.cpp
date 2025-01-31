#include "Helicopter.h"
#include <iostream>

Helicopter::Helicopter(const std::string& name) : name(name), health(100) {}

void Helicopter::attack(const std::string& target) {
    std::cout << name << " is attacking " << target << "!\n";
    // Implement attack logic
}

void Helicopter::addWeapon(const Weapon& weapon) {
    weapons.push_back(weapon);
}

void Helicopter::showStatus() const {
    std::cout << "Helicopter: " << name << "\nHealth: " << health << "\n";
    for (const auto& weapon : weapons) {
        std::cout << "Weapon: " << weapon.getName() << "\n";
    }
}
#include "Game.h"

int main() {
    Game game;
    game.start();
    return 0;
}