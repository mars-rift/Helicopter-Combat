#pragma once
#include <string>
#include <vector>
#include "Weapon.h"

class Helicopter {
public:
    Helicopter(const std::string& name);
    void attack(const std::string& target);
    void addWeapon(const Weapon& weapon);
    void showStatus() const;

private:
    std::string name;
    std::vector<Weapon> weapons;
    int health;
};
