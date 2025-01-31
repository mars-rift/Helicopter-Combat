#pragma once
#include <string>
#include <random>

class Weapon {
public:
    Weapon(const std::string& name, int minDamage, int maxDamage);
    std::string getName() const;
    int getDamage() const;

private:
    std::string name;
    int minDamage;
    int maxDamage;
    mutable std::mt19937 rng;
    mutable std::uniform_int_distribution<int> dist;
};
