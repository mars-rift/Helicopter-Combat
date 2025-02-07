#pragma once
#include <string>
#include <vector>
#include "Weapon.h"
#include "Enemy.h"

class Helicopter {
public:
    Helicopter(const std::string& name);
    Helicopter();
    void attack(Enemy& target);
    void addWeapon(const Weapon& weapon);
    void showStatus() const;

private:
    std::string name;
    std::vector<Weapon> weapons;
    int health;
};
