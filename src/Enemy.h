#pragma once
#include <string>
#include <random>

class Enemy {
public:
    Enemy(const std::string& type, int health, int minDamage, int maxDamage);
    Enemy(const std::string& type, int health); // Add this line
    std::string getType() const;
    int getHealth() const;
    void takeDamage(int damage);
    int attackDamage() const; // New method to calculate enemy attack damage

private:
    std::string type;
    int health;
    int minDamage;
    int maxDamage;
    mutable std::mt19937 rng;
    mutable std::uniform_int_distribution<int> damageRange;
};
