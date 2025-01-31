#include "Enemy.h"

Enemy::Enemy(const std::string& type, int health) : type(type), health(health) {}

std::string Enemy::getType() const {
    return type;
}

int Enemy::getHealth() const {
    return health;
}

void Enemy::takeDamage(int damage) {
    health -= damage;
}
