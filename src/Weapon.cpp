#include "Weapon.h"

Weapon::Weapon(const std::string& name, int minDamage, int maxDamage)
    : name(name), minDamage(minDamage), maxDamage(maxDamage), rng(std::random_device{}()), dist(minDamage, maxDamage) {
}

std::string Weapon::getName() const {
    return name;
}

int Weapon::getDamage() const {
    return dist(rng);
}
