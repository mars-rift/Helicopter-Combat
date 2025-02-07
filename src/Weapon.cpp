#include "Weapon.h"

Weapon::Weapon(const std::string& name, int minDamage, int maxDamage)
    : name(name), minDamage(minDamage), maxDamage(maxDamage), dist(minDamage, maxDamage) {
    std::random_device rd;
    rng.seed(rd());
}

std::string Weapon::getName() const {
    return name;
}

int Weapon::getDamage() const {
    return dist(rng);
}
