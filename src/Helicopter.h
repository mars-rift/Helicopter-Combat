#pragma once
#include <string>
#include <vector>
#include "Weapon.h"
#include "Enemy.h"

class Helicopter {
public:
    Helicopter(const std::string& name);
    void attack(Enemy& target);
    void addWeapon(const Weapon& weapon);
    void showStatus() const;
    void printDamageTable() const;

private:
    std::string name;
    std::vector<Weapon> weapons;
    int health;
    std::vector<std::pair<std::string, int>> damageTable; // Store weapon name and damage dealt
};
