#include "Enemy.h"
#include <random>
#include <iostream>
#include <cmath>
#include <iomanip>

Enemy::Enemy(const std::string& type, int health, int minDamage, int maxDamage)
    : type(type), enemyType(EnemyType::SCOUT_DRONE), health(health), maxHealth(health),
      minDamage(minDamage), maxDamage(maxDamage), position(0, 0, 0, 0),
      behavior(EnemyBehavior::PATROL), currentPatrolPoint(0), isAlerted(false),
      alertLevel(0.0), lastSeenTarget(0.0), moveSpeed(20.0), isEngaging(false),
      rng(std::random_device{}()), damageRange(minDamage, maxDamage) {
    
    initializeCapabilities();
}

Enemy::Enemy(const std::string& type, int health)
    : type(type), enemyType(EnemyType::SCOUT_DRONE), health(health), maxHealth(health),
      minDamage(10), maxDamage(20), position(0, 0, 0, 0),
      behavior(EnemyBehavior::PATROL), currentPatrolPoint(0), isAlerted(false),
      alertLevel(0.0), lastSeenTarget(0.0), moveSpeed(20.0), isEngaging(false),
      rng(std::random_device{}()), damageRange(10, 20) {
    
    initializeCapabilities();
}

Enemy::Enemy(EnemyType type, const EnemyPosition& pos)
    : enemyType(type), position(pos), behavior(EnemyBehavior::PATROL),
      currentPatrolPoint(0), isAlerted(false), alertLevel(0.0), lastSeenTarget(0.0),
      moveSpeed(20.0), isEngaging(false), rng(std::random_device{}()) {
    
    // Set basic attributes based on enemy type
    switch (type) {
        case EnemyType::SCOUT_DRONE:
            this->type = "Scout Drone";
            health = maxHealth = 30;
            minDamage = 5; maxDamage = 15;
            moveSpeed = 80.0;
            break;
        case EnemyType::ATTACK_DRONE:
            this->type = "Attack Drone";
            health = maxHealth = 50;
            minDamage = 10; maxDamage = 25;
            moveSpeed = 60.0;
            break;
        case EnemyType::LIGHT_TANK:
            this->type = "Light Tank";
            health = maxHealth = 100;
            minDamage = 20; maxDamage = 35;
            moveSpeed = 40.0;
            break;
        case EnemyType::HEAVY_TANK:
            this->type = "Heavy Tank";
            health = maxHealth = 180;
            minDamage = 35; maxDamage = 50;
            moveSpeed = 25.0;
            break;
        case EnemyType::SAM_SITE:
            this->type = "SAM Site";
            health = maxHealth = 80;
            minDamage = 40; maxDamage = 70;
            moveSpeed = 0.0; // Stationary
            break;
        case EnemyType::FIGHTER_JET:
            this->type = "Fighter Jet";
            health = maxHealth = 120;
            minDamage = 45; maxDamage = 80;
            moveSpeed = 500.0;
            break;
        case EnemyType::ATTACK_HELICOPTER:
            this->type = "Attack Helicopter";
            health = maxHealth = 90;
            minDamage = 25; maxDamage = 45;
            moveSpeed = 200.0;
            break;
        case EnemyType::MOBILE_AAA:
            this->type = "Mobile AAA";
            health = maxHealth = 70;
            minDamage = 15; maxDamage = 30;
            moveSpeed = 35.0;
            break;
    }
    
    damageRange = std::uniform_int_distribution<int>(minDamage, maxDamage);
    initializeCapabilities();
}

void Enemy::initializeCapabilities() {
    switch (enemyType) {
        case EnemyType::SCOUT_DRONE:
            capabilities.detectionRange = 8.0;
            capabilities.engagementRange = 3.0;
            capabilities.maxSpeed = 80.0;
            capabilities.armor = 0.1;
            capabilities.hasRadar = true;
            capabilities.isAirborne = true;
            capabilities.canMove = true;
            break;
        case EnemyType::ATTACK_DRONE:
            capabilities.detectionRange = 6.0;
            capabilities.engagementRange = 4.0;
            capabilities.maxSpeed = 60.0;
            capabilities.armor = 0.2;
            capabilities.hasRadar = true;
            capabilities.isAirborne = true;
            capabilities.canMove = true;
            break;
        case EnemyType::LIGHT_TANK:
            capabilities.detectionRange = 5.0;
            capabilities.engagementRange = 6.0;
            capabilities.maxSpeed = 40.0;
            capabilities.armor = 0.4;
            capabilities.hasRadar = false;
            capabilities.isAirborne = false;
            capabilities.canMove = true;
            break;
        case EnemyType::HEAVY_TANK:
            capabilities.detectionRange = 4.0;
            capabilities.engagementRange = 8.0;
            capabilities.maxSpeed = 25.0;
            capabilities.armor = 0.6;
            capabilities.hasRadar = false;
            capabilities.isAirborne = false;
            capabilities.canMove = true;
            break;
        case EnemyType::SAM_SITE:
            capabilities.detectionRange = 15.0;
            capabilities.engagementRange = 12.0;
            capabilities.maxSpeed = 0.0;
            capabilities.armor = 0.3;
            capabilities.hasRadar = true;
            capabilities.isAirborne = false;
            capabilities.canMove = false;
            break;
        case EnemyType::FIGHTER_JET:
            capabilities.detectionRange = 20.0;
            capabilities.engagementRange = 15.0;
            capabilities.maxSpeed = 500.0;
            capabilities.armor = 0.2;
            capabilities.hasRadar = true;
            capabilities.isAirborne = true;
            capabilities.canMove = true;
            break;
        case EnemyType::ATTACK_HELICOPTER:
            capabilities.detectionRange = 10.0;
            capabilities.engagementRange = 8.0;
            capabilities.maxSpeed = 200.0;
            capabilities.armor = 0.3;
            capabilities.hasRadar = true;
            capabilities.isAirborne = true;
            capabilities.canMove = true;
            break;
        case EnemyType::MOBILE_AAA:
            capabilities.detectionRange = 8.0;
            capabilities.engagementRange = 6.0;
            capabilities.maxSpeed = 35.0;
            capabilities.armor = 0.3;
            capabilities.hasRadar = true;
            capabilities.isAirborne = false;
            capabilities.canMove = true;
            break;
    }
}

std::string Enemy::getType() const {
    return type;
}

int Enemy::getHealth() const {
    return health;
}

void Enemy::takeDamage(int damage) {
    // Apply armor reduction
    double actualDamage = damage * (1.0 - capabilities.armor);
    health -= static_cast<int>(actualDamage);
    
    if (health < 0) health = 0;
    
    std::cout << type << " takes " << static_cast<int>(actualDamage) 
              << " damage (armor reduced from " << damage << ")" << std::endl;
    
    // Become alerted when taking damage
    isAlerted = true;
    alertLevel = 1.0;
}

int Enemy::attackDamage() const {
    return damageRange(rng);
}

void Enemy::updatePosition(double deltaTime) {
    if (!capabilities.canMove) return;
    
    updateAI(deltaTime);
    
    if (behavior == EnemyBehavior::PATROL && !patrolRoute.empty()) {
        updatePatrol(deltaTime);
    }
}

void Enemy::updateAI(double deltaTime) {
    // Simple AI logic
    if (isAlerted) {
        alertLevel += deltaTime * 0.1; // Increase alertness over time
        lastSeenTarget += deltaTime;
        
        // Return to patrol if no contact for a while
        if (lastSeenTarget > 30.0) {
            isAlerted = false;
            alertLevel = 0.0;
            behavior = EnemyBehavior::PATROL;
        }
    }
}

void Enemy::updatePatrol(double deltaTime) {
    if (patrolRoute.empty()) return;
    
    EnemyPosition& target = patrolRoute[currentPatrolPoint];
    double distance = calculateDistance(position, target);
    
    if (distance < 0.5) { // Reached waypoint
        currentPatrolPoint = (currentPatrolPoint + 1) % patrolRoute.size();
    } else {
        // Move towards waypoint
        double dx = target.x - position.x;
        double dy = target.y - position.y;
        double moveDistance = (moveSpeed / 3600.0) * deltaTime; // km
        
        if (distance > 0) {
            position.x += (dx / distance) * moveDistance;
            position.y += (dy / distance) * moveDistance;
        }
    }
}

double Enemy::calculateDistance(const EnemyPosition& pos1, const EnemyPosition& pos2) const {
    double dx = pos1.x - pos2.x;
    double dy = pos1.y - pos2.y;
    return sqrt(dx*dx + dy*dy);
}

bool Enemy::detectTarget(const EnemyPosition& targetPos, double stealthFactor) const {
    double distance = calculateDistance(position, targetPos);
    
    if (distance > capabilities.detectionRange) return false;
    
    // Detection probability based on distance and stealth
    double detectionChance = 1.0 - (distance / capabilities.detectionRange);
    detectionChance *= (1.0 - stealthFactor);
    
    // Radar bonus
    if (capabilities.hasRadar) {
        detectionChance *= 1.5;
    }
    
    std::uniform_real_distribution<double> detectRoll(0.0, 1.0);
    return detectRoll(rng) < detectionChance;
}

bool Enemy::canEngageTarget(const EnemyPosition& targetPos) const {
    double distance = calculateDistance(position, targetPos);
    return distance <= capabilities.engagementRange;
}

double Enemy::calculateHitProbability(double distance, double targetSpeed) const {
    double hitChance = 0.8; // Base hit chance
    
    // Range factor
    hitChance *= (1.0 - distance / capabilities.engagementRange);
    
    // Target speed factor
    hitChance *= (1.0 / (1.0 + targetSpeed / 100.0));
    
    // Radar bonus for guided systems
    if (capabilities.hasRadar) {
        hitChance *= 1.2;
    }
    
    return std::max(0.0, std::min(1.0, hitChance));
}

void Enemy::performAttack(const EnemyPosition& targetPos) const {
    double distance = calculateDistance(position, targetPos);
    
    if (canEngageTarget(targetPos)) {
        std::cout << type << " engages target at " << distance << "km range!" << std::endl;
    }
}

void Enemy::setPatrolRoute(const std::vector<EnemyPosition>& route) {
    patrolRoute = route;
    currentPatrolPoint = 0;
}

void Enemy::setBehavior(EnemyBehavior newBehavior) {
    behavior = newBehavior;
}

void Enemy::reactToThreat(const EnemyPosition& threatPos) {
    isAlerted = true;
    alertLevel = 1.0;
    lastSeenTarget = 0.0;
    targetPosition = threatPos;
    
    // Change behavior based on enemy type
    switch (enemyType) {
        case EnemyType::SCOUT_DRONE:
            behavior = EnemyBehavior::EVASIVE;
            break;
        case EnemyType::ATTACK_DRONE:
        case EnemyType::FIGHTER_JET:
        case EnemyType::ATTACK_HELICOPTER:
            behavior = EnemyBehavior::AGGRESSIVE;
            break;
        default:
            behavior = EnemyBehavior::DEFENSIVE;
            break;
    }
}

bool Enemy::hasLineOfSight(const EnemyPosition& targetPos) const {
    // Simplified line of sight calculation
    double distance = calculateDistance(position, targetPos);
    
    // Air targets generally have good line of sight
    if (capabilities.isAirborne) return true;
    
    // Ground targets may be blocked by terrain
    return distance < capabilities.detectionRange * 0.8;
}

void Enemy::showDetailedStatus() const {
    std::cout << "\n=== " << type << " STATUS ===" << std::endl;
    std::cout << "Health: " << health << "/" << maxHealth << std::endl;
    std::cout << "Position: (" << position.x << ", " << position.y << ") at " << position.altitude << "m" << std::endl;
    std::cout << "Behavior: ";
    switch (behavior) {
        case EnemyBehavior::PATROL: std::cout << "Patrol"; break;
        case EnemyBehavior::AGGRESSIVE: std::cout << "Aggressive"; break;
        case EnemyBehavior::DEFENSIVE: std::cout << "Defensive"; break;
        case EnemyBehavior::EVASIVE: std::cout << "Evasive"; break;
        case EnemyBehavior::FORMATION: std::cout << "Formation"; break;
    }
    std::cout << std::endl;
    std::cout << "Alert Level: " << std::fixed << std::setprecision(0) << alertLevel * 100 << "%" << std::endl;
    std::cout << "Detection Range: " << capabilities.detectionRange << "km" << std::endl;
    std::cout << "Engagement Range: " << capabilities.engagementRange << "km" << std::endl;
}

void Enemy::joinFormation(const std::vector<Enemy*>& formation) {
    behavior = EnemyBehavior::FORMATION;
    // Formation logic would be implemented here
}

void Enemy::coordinateAttack(const std::vector<Enemy*>& allies, const EnemyPosition& target) {
    // Coordinate with other units for synchronized attack
    for (auto* ally : allies) {
        if (ally != this && ally->canEngageTarget(target)) {
            ally->setBehavior(EnemyBehavior::AGGRESSIVE);
        }
    }
}

EnemyPosition Enemy::calculateInterceptPosition(const EnemyPosition& targetPos, double targetSpeed) const {
    // Simple intercept calculation
    double distance = calculateDistance(position, targetPos);
    double timeToIntercept = distance / capabilities.maxSpeed;
    
    // Predict where target will be
    EnemyPosition interceptPos = targetPos;
    interceptPos.x += targetSpeed * timeToIntercept * cos(targetPos.heading * M_PI / 180.0);
    interceptPos.y += targetSpeed * timeToIntercept * sin(targetPos.heading * M_PI / 180.0);
    
    return interceptPos;
}
