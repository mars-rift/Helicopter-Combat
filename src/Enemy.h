#pragma once
#include <string>

class Enemy {
public:
    Enemy(const std::string& type, int health);
    std::string getType() const;
    int getHealth() const;
    void takeDamage(int damage);

private:
    std::string type;
    int health;
};
