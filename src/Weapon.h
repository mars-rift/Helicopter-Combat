#pragma once
#include <string>
#include <random>

enum class WeaponType {
    AIR_TO_AIR_MISSILE,
    AIR_TO_GROUND_MISSILE,
    MACHINE_GUN,
    ROCKET_POD,
    CANNON,
    GUIDED_MISSILE,
    UNGUIDED_ROCKET
};

enum class GuidanceType {
    NONE,               // Unguided
    INFRARED,          // Heat-seeking
    RADAR,             // Radar-guided
    LASER,             // Laser-guided
    GPS                // GPS-guided
};

struct WeaponSpecs {
    double range;           // km
    double lockOnTime;      // seconds
    double reloadTime;      // seconds
    double accuracy;        // 0.0 to 1.0
    bool requiresLOS;       // Line of sight
    double penetration;     // armor penetration capability
    double blastRadius;     // meters
};

class Weapon {
public:
    Weapon(const std::string& name, int minDamage, int maxDamage);
    Weapon(const std::string& name, WeaponType type, int minDamage, int maxDamage, 
           int ammoCount, const WeaponSpecs& specs);
    
    // Basic properties
    std::string getName() const;
    int getDamage() const;
    WeaponType getType() const { return weaponType; }
    GuidanceType getGuidanceType() const { return guidanceType; }
    
    // Ammunition management
    int getAmmoCount() const { return currentAmmo; }
    int getMaxAmmo() const { return maxAmmo; }
    bool hasAmmo() const { return currentAmmo > 0; }
    void reload();
    bool consumeAmmo();
    
    // Combat mechanics
    bool canEngageTarget(double distance, bool hasLineOfSight) const;
    double calculateHitProbability(double distance, double targetSpeed, 
                                   double weatherEffect = 1.0) const;
    bool requiresLockOn() const;
    double getLockOnTime() const { return specs.lockOnTime; }
    bool isReloading() const { return reloadTimeRemaining > 0; }
    
    // Targeting and guidance
    bool lockOnTarget(double distance, double targetSpeed, bool hasRadar);
    void updateGuidance(double deltaTime);
    bool isLockedOn() const { return lockedOn; }
    
    // Status and updates
    void update(double deltaTime);
    void showWeaponInfo() const;
    bool isReady() const { return !isReloading() && hasAmmo(); }
    
    // Effectiveness calculations
    double calculateDamageAtRange(double distance) const;
    bool isEffectiveAgainst(const std::string& targetType) const;
    double getArmorPenetration() const { return specs.penetration; }

private:
    // Core attributes
    std::string name;
    WeaponType weaponType;
    GuidanceType guidanceType;
    int minDamage;
    int maxDamage;
    
    // Ammunition
    int currentAmmo;
    int maxAmmo;
    double reloadTimeRemaining;
    
    // Technical specifications
    WeaponSpecs specs;
    
    // Targeting state
    bool lockedOn;
    double lockOnProgress;  // 0.0 to 1.0
    
    // Random generation
    mutable std::mt19937 rng;
    mutable std::uniform_int_distribution<int> dist;
    
    // Private methods
    void initializeGuidanceType();
    double calculateRangeDamageReduction(double distance) const;
};
