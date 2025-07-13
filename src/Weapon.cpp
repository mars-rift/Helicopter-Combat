#include "Weapon.h"
#include <iostream>
#include <iomanip>
#include <cmath>

Weapon::Weapon(const std::string& name, int minDamage, int maxDamage)
    : name(name), weaponType(WeaponType::MACHINE_GUN), guidanceType(GuidanceType::NONE),
      minDamage(minDamage), maxDamage(maxDamage), currentAmmo(100), maxAmmo(100),
      reloadTimeRemaining(0.0), lockedOn(false), lockOnProgress(0.0),
      rng(std::random_device{}()), dist(minDamage, maxDamage) {
    
    // Default specifications
    specs.range = 2.0;           // 2km default range
    specs.lockOnTime = 0.0;      // No lock-on time for unguided weapons
    specs.reloadTime = 3.0;      // 3 seconds reload
    specs.accuracy = 0.8;        // 80% base accuracy
    specs.requiresLOS = true;    // Requires line of sight
    specs.penetration = 20.0;    // Basic penetration
    specs.blastRadius = 5.0;     // 5m blast radius
    
    initializeGuidanceType();
}

Weapon::Weapon(const std::string& name, WeaponType type, int minDamage, int maxDamage, 
               int ammoCount, const WeaponSpecs& weaponSpecs)
    : name(name), weaponType(type), minDamage(minDamage), maxDamage(maxDamage),
      currentAmmo(ammoCount), maxAmmo(ammoCount), reloadTimeRemaining(0.0),
      specs(weaponSpecs), lockedOn(false), lockOnProgress(0.0),
      rng(std::random_device{}()), dist(minDamage, maxDamage) {
    
    initializeGuidanceType();
}

void Weapon::initializeGuidanceType() {
    switch (weaponType) {
        case WeaponType::AIR_TO_AIR_MISSILE:
            guidanceType = GuidanceType::INFRARED;
            specs.lockOnTime = 3.0;
            specs.range = 15.0;
            break;
        case WeaponType::AIR_TO_GROUND_MISSILE:
            guidanceType = GuidanceType::LASER;
            specs.lockOnTime = 2.0;
            specs.range = 8.0;
            break;
        case WeaponType::GUIDED_MISSILE:
            guidanceType = GuidanceType::RADAR;
            specs.lockOnTime = 4.0;
            specs.range = 20.0;
            break;
        case WeaponType::MACHINE_GUN:
        case WeaponType::CANNON:
        case WeaponType::ROCKET_POD:
        case WeaponType::UNGUIDED_ROCKET:
        default:
            guidanceType = GuidanceType::NONE;
            specs.lockOnTime = 0.0;
            break;
    }
}

std::string Weapon::getName() const {
    return name;
}

int Weapon::getDamage() const {
    return dist(rng);
}

bool Weapon::consumeAmmo() {
    if (currentAmmo > 0) {
        currentAmmo--;
        return true;
    }
    return false;
}

void Weapon::reload() {
    if (currentAmmo < maxAmmo) {
        reloadTimeRemaining = specs.reloadTime;
        std::cout << "Reloading " << name << "..." << std::endl;
    }
}

bool Weapon::canEngageTarget(double distance, bool hasLineOfSight) const {
    if (distance > specs.range) return false;
    if (specs.requiresLOS && !hasLineOfSight) return false;
    if (!hasAmmo()) return false;
    if (isReloading()) return false;
    
    return true;
}

double Weapon::calculateHitProbability(double distance, double targetSpeed, double weatherEffect) const {
    // Base accuracy
    double hitChance = specs.accuracy;
    
    // Distance factor
    double rangeFactor = 1.0 - (distance / specs.range);
    hitChance *= rangeFactor;
    
    // Target speed factor (harder to hit fast targets)
    double speedFactor = 1.0 / (1.0 + targetSpeed / 100.0);
    hitChance *= speedFactor;
    
    // Weather effect
    hitChance *= weatherEffect;
    
    // Guidance system bonus
    switch (guidanceType) {
        case GuidanceType::INFRARED:
            hitChance *= 1.3;
            break;
        case GuidanceType::RADAR:
            hitChance *= 1.4;
            break;
        case GuidanceType::LASER:
            hitChance *= 1.5;
            break;
        case GuidanceType::GPS:
            hitChance *= 1.2;
            break;
        case GuidanceType::NONE:
        default:
            break;
    }
    
    // Ensure probability stays within bounds
    return std::max(0.0, std::min(1.0, hitChance));
}

bool Weapon::requiresLockOn() const {
    return specs.lockOnTime > 0.0;
}

bool Weapon::lockOnTarget(double distance, double targetSpeed, bool hasRadar) {
    if (!requiresLockOn()) {
        lockedOn = true;
        return true;
    }
    
    if (distance > specs.range) return false;
    
    // Some guidance types require radar
    if ((guidanceType == GuidanceType::RADAR) && !hasRadar) {
        return false;
    }
    
    lockOnProgress = 0.0;
    std::cout << "Acquiring lock with " << name << "..." << std::endl;
    return true;
}

void Weapon::updateGuidance(double deltaTime) {
    if (requiresLockOn() && !lockedOn) {
        lockOnProgress += deltaTime;
        if (lockOnProgress >= specs.lockOnTime) {
            lockedOn = true;
            std::cout << "Target locked! " << name << " ready to fire." << std::endl;
        }
    }
}

void Weapon::update(double deltaTime) {
    // Update reload timer
    if (reloadTimeRemaining > 0) {
        reloadTimeRemaining -= deltaTime;
        if (reloadTimeRemaining <= 0) {
            reloadTimeRemaining = 0;
            currentAmmo = maxAmmo;
            std::cout << name << " reloaded!" << std::endl;
        }
    }
    
    // Update guidance system
    updateGuidance(deltaTime);
}

double Weapon::calculateDamageAtRange(double distance) const {
    if (distance <= 0) return 1.0;
    
    // Linear damage reduction with range
    double rangeFactor = 1.0 - (distance / specs.range);
    rangeFactor = std::max(0.5, rangeFactor); // Minimum 50% damage at max range
    
    return rangeFactor;
}

bool Weapon::isEffectiveAgainst(const std::string& targetType) const {
    switch (weaponType) {
        case WeaponType::AIR_TO_AIR_MISSILE:
            return (targetType.find("Drone") != std::string::npos || 
                    targetType.find("Helicopter") != std::string::npos ||
                    targetType.find("Jet") != std::string::npos);
        
        case WeaponType::AIR_TO_GROUND_MISSILE:
            return (targetType.find("Tank") != std::string::npos ||
                    targetType.find("SAM") != std::string::npos ||
                    targetType.find("AAA") != std::string::npos);
        
        case WeaponType::MACHINE_GUN:
        case WeaponType::CANNON:
            return (targetType.find("Drone") != std::string::npos ||
                    targetType.find("Light") != std::string::npos);
        
        case WeaponType::ROCKET_POD:
        case WeaponType::UNGUIDED_ROCKET:
            return true; // Effective against most targets
        
        case WeaponType::GUIDED_MISSILE:
            return true; // Versatile weapon
        
        default:
            return true;
    }
}

void Weapon::showWeaponInfo() const {
    std::cout << "\n=== " << name << " ===" << std::endl;
    std::cout << "Type: ";
    switch (weaponType) {
        case WeaponType::AIR_TO_AIR_MISSILE: std::cout << "Air-to-Air Missile"; break;
        case WeaponType::AIR_TO_GROUND_MISSILE: std::cout << "Air-to-Ground Missile"; break;
        case WeaponType::MACHINE_GUN: std::cout << "Machine Gun"; break;
        case WeaponType::ROCKET_POD: std::cout << "Rocket Pod"; break;
        case WeaponType::CANNON: std::cout << "Cannon"; break;
        case WeaponType::GUIDED_MISSILE: std::cout << "Guided Missile"; break;
        case WeaponType::UNGUIDED_ROCKET: std::cout << "Unguided Rocket"; break;
    }
    std::cout << std::endl;
    
    std::cout << "Damage: " << minDamage << "-" << maxDamage << std::endl;
    std::cout << "Range: " << std::fixed << std::setprecision(1) << specs.range << " km" << std::endl;
    std::cout << "Accuracy: " << std::fixed << std::setprecision(0) << specs.accuracy * 100 << "%" << std::endl;
    std::cout << "Ammunition: " << currentAmmo << "/" << maxAmmo << std::endl;
    
    if (requiresLockOn()) {
        std::cout << "Lock-on time: " << std::fixed << std::setprecision(1) << specs.lockOnTime << " seconds" << std::endl;
        std::cout << "Guidance: ";
        switch (guidanceType) {
            case GuidanceType::INFRARED: std::cout << "Infrared"; break;
            case GuidanceType::RADAR: std::cout << "Radar"; break;
            case GuidanceType::LASER: std::cout << "Laser"; break;
            case GuidanceType::GPS: std::cout << "GPS"; break;
            case GuidanceType::NONE: std::cout << "None"; break;
        }
        std::cout << std::endl;
    }
    
    if (isReloading()) {
        std::cout << "Status: Reloading (" << std::fixed << std::setprecision(1) 
                  << reloadTimeRemaining << "s remaining)" << std::endl;
    } else if (!hasAmmo()) {
        std::cout << "Status: No ammunition" << std::endl;
    } else {
        std::cout << "Status: Ready" << std::endl;
    }
}
