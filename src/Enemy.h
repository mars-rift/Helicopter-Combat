#pragma once
#include <string>
#include <random>

enum class EnemyType {
    SCOUT_DRONE,
    ATTACK_DRONE,
    LIGHT_TANK,
    HEAVY_TANK,
    SAM_SITE,
    FIGHTER_JET,
    ATTACK_HELICOPTER,
    MOBILE_AAA
};

enum class EnemyBehavior {
    PATROL,
    AGGRESSIVE,
    DEFENSIVE,
    EVASIVE,
    FORMATION
};

struct EnemyPosition {
    double x, y, altitude;
    double heading;  // degrees
    EnemyPosition(double x = 0, double y = 0, double alt = 0, double h = 0) 
        : x(x), y(y), altitude(alt), heading(h) {}
};

struct EnemyCapabilities {
    double detectionRange;   // km
    double engagementRange;  // km
    double maxSpeed;         // km/h
    double armor;           // damage reduction factor
    bool hasRadar;
    bool isAirborne;
    bool canMove;
};

class Enemy {
public:
    Enemy(const std::string& type, int health, int minDamage, int maxDamage);
    Enemy(const std::string& type, int health);
    Enemy(EnemyType type, const EnemyPosition& pos);
    
    // Basic properties
    std::string getType() const;
    int getHealth() const;
    int getMaxHealth() const { return maxHealth; }
    void takeDamage(int damage);
    bool isAlive() const { return health > 0; }
    
    // Combat methods
    int attackDamage() const;
    bool canEngageTarget(const EnemyPosition& targetPos) const;
    double calculateHitProbability(double distance, double targetSpeed) const;
    void performAttack(const EnemyPosition& targetPos) const;
    
    // AI and movement
    void updatePosition(double deltaTime);
    void setPatrolRoute(const std::vector<EnemyPosition>& route);
    void setBehavior(EnemyBehavior behavior);
    void reactToThreat(const EnemyPosition& threatPos);
    
    // Detection and awareness
    bool detectTarget(const EnemyPosition& targetPos, double stealthFactor) const;
    double getDetectionRange() const { return capabilities.detectionRange; }
    bool hasLineOfSight(const EnemyPosition& targetPos) const;
    
    // Status and information
    const EnemyPosition& getPosition() const { return position; }
    const EnemyCapabilities& getCapabilities() const { return capabilities; }
    EnemyBehavior getBehavior() const { return behavior; }
    bool isAirTarget() const { return capabilities.isAirborne; }
    void showDetailedStatus() const;
    
    // Formation and coordination
    void joinFormation(const std::vector<Enemy*>& formation);
    void coordinateAttack(const std::vector<Enemy*>& allies, const EnemyPosition& target);

private:
    // Core attributes
    std::string type;
    EnemyType enemyType;
    int health;
    int maxHealth;
    int minDamage;
    int maxDamage;
    EnemyPosition position;
    EnemyCapabilities capabilities;
    EnemyBehavior behavior;
    
    // AI state
    std::vector<EnemyPosition> patrolRoute;
    size_t currentPatrolPoint;
    bool isAlerted;
    double alertLevel;      // 0.0 to 1.0
    double lastSeenTarget;  // time since last detection
    
    // Movement and targeting
    EnemyPosition targetPosition;
    double moveSpeed;
    bool isEngaging;
    
    // Random generation
    mutable std::mt19937 rng;
    mutable std::uniform_int_distribution<int> damageRange;
    
    // Private helper methods
    void initializeCapabilities();
    void updateAI(double deltaTime);
    void updatePatrol(double deltaTime);
    double calculateDistance(const EnemyPosition& pos1, const EnemyPosition& pos2) const;
    EnemyPosition calculateInterceptPosition(const EnemyPosition& targetPos, double targetSpeed) const;
};
