#include "Enemy.h"
#include <random>

Enemy::Enemy(const std::string& type, int health, int minDamage, int maxDamage)
    : type(type), health(health), minDamage(minDamage), maxDamage(maxDamage),
    damageRange(minDamage, maxDamage) {
    std::random_device rd;
    rng.seed(rd());
}

Enemy::Enemy(const std::string& type, int health)
    : type(type), health(health), minDamage(0), maxDamage(0), damageRange(0, 0) {
}

int Enemy::attackDamage() const {
    return damageRange(rng);
}

std::string Enemy::getType() const {
    return type;
}

int Enemy::getHealth() const {
    return health;
}

void Enemy::takeDamage(int damage) {
    health -= damage;
}
