#include <iostream>
#include <random>
#include <iomanip>
#include <cmath>
#include "Helicopter.h"

Helicopter::Helicopter(const std::string& name) 
    : name(name), health(100.0), position(0, 0, 100), 
      radarRange(25.0), visualRange(5.0), stealthFactor(0.3),
      isHovering(false), timeInMission(0.0) {
    
    // Initialize flight parameters
    flightParams.speed = 0.0;
    flightParams.maxSpeed = 300.0;  // km/h
    flightParams.fuel = 800.0;      // liters
    flightParams.maxFuel = 800.0;
    flightParams.fuelConsumption = 4.5; // liters per minute
    flightParams.maneuverability = 0.8;
    
    // Initialize all systems as operational
    systems.engine = true;
    systems.mainRotor = true;
    systems.tailRotor = true;
    systems.radar = true;
    systems.navigation = true;
    systems.weaponSystems = true;
    systems.engineHealth = 1.0;
    systems.rotorHealth = 1.0;
    systems.radarHealth = 1.0;
}

Helicopter::Helicopter() : Helicopter("AH-64 Apache") {}

void Helicopter::attack(Enemy& target) {
    if (weapons.empty()) {
        std::cout << name << " has no weapons to attack " << target.getType() << "!" << std::endl;
        return;
    }

    // Use first available weapon with ammo
    for (size_t i = 0; i < weapons.size(); ++i) {
        if (weapons[i].hasAmmo()) {
            double distance = 2.0; // Assume 2km for basic attack
            attackWithWeapon(target, static_cast<int>(i), distance);
            break;
        }
    }
}

bool Helicopter::attackWithWeapon(Enemy& target, int weaponIndex, double distance) {
    if (weaponIndex < 0 || weaponIndex >= static_cast<int>(weapons.size())) {
        std::cout << "Invalid weapon selection." << std::endl;
        return false;
    }

    if (!systems.weaponSystems) {
        std::cout << "Weapon systems offline! Cannot engage target." << std::endl;
        return false;
    }

    Weapon& weapon = weapons[weaponIndex];
    
    if (!weapon.hasAmmo()) {
        std::cout << weapon.getName() << " has no ammunition remaining!" << std::endl;
        return false;
    }

    if (!weapon.canEngageTarget(distance, true)) {
        std::cout << target.getType() << " is out of range for " << weapon.getName() << std::endl;
        return false;
    }

    // Calculate hit probability based on distance, weather, and system health
    double hitChance = weapon.calculateHitProbability(distance, 0.0, 1.0);
    hitChance *= systems.radarHealth; // Degraded systems reduce accuracy
    
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_real_distribution<double> hitRoll(0.0, 1.0);
    
    weapon.consumeAmmo();
    
    if (hitRoll(rng) <= hitChance) {
        int damage = weapon.getDamage();
        damage = static_cast<int>(damage * weapon.calculateDamageAtRange(distance));
        
        std::cout << name << " successfully hits " << target.getType()
                  << " with " << weapon.getName()
                  << " causing " << damage << " damage at " << distance << "km!" << std::endl;
        target.takeDamage(damage);
        return true;
    } else {
        std::cout << name << " misses " << target.getType() 
                  << " with " << weapon.getName() << "!" << std::endl;
        return false;
    }
}

void Helicopter::takeDamage(int damage, const std::string& component) {
    double actualDamage = damage;
    
    // Apply damage to specific component
    if (component == "engine") {
        systems.engineHealth -= damage / 200.0;
        if (systems.engineHealth <= 0) {
            systems.engine = false;
            systems.engineHealth = 0;
            std::cout << "*** ENGINE CRITICAL DAMAGE! ***" << std::endl;
        }
    } else if (component == "rotor") {
        systems.rotorHealth -= damage / 150.0;
        if (systems.rotorHealth <= 0.3) {
            flightParams.maneuverability *= 0.5;
            std::cout << "*** ROTOR DAMAGE - Reduced maneuverability! ***" << std::endl;
        }
    } else if (component == "radar") {
        systems.radarHealth -= damage / 100.0;
        if (systems.radarHealth <= 0) {
            systems.radar = false;
            systems.radarHealth = 0;
            std::cout << "*** RADAR SYSTEM DESTROYED! ***" << std::endl;
        }
    }
    
    // Apply general hull damage
    health -= actualDamage;
    if (health < 0) health = 0;
    
    std::cout << name << " takes " << damage << " damage to " << component 
              << "! Hull integrity: " << std::fixed << std::setprecision(1) 
              << health << "%" << std::endl;
    
    // Check for critical system failures
    if (health <= 25.0 && health > 0) {
        std::cout << "*** WARNING: Critical damage sustained! ***" << std::endl;
    }
}

bool Helicopter::isAlive() const {
    return health > 0 && canFly();
}

bool Helicopter::canFly() const {
    return systems.engine && systems.mainRotor && 
           flightParams.fuel > 0 && health > 10.0;
}

void Helicopter::updatePosition(double deltaTime) {
    if (!canFly()) {
        if (position.altitude > 0) {
            // Emergency landing
            position.altitude -= 50.0 * deltaTime; // 50m/s descent
            if (position.altitude <= 0) {
                position.altitude = 0;
                std::cout << "*** EMERGENCY LANDING COMPLETED ***" << std::endl;
            }
        }
        return;
    }
    
    updateFuel(deltaTime);
    
    if (!isHovering) {
        // Move towards destination
        double dx = destination.x - position.x;
        double dy = destination.y - position.y;
        double dalt = destination.altitude - position.altitude;
        
        double distance = sqrt(dx*dx + dy*dy + dalt*dalt);
        
        if (distance > 0.1) { // 100m threshold
            double moveDistance = (flightParams.speed / 3600.0) * deltaTime; // km
            double ratio = std::min(1.0, moveDistance / distance);
            
            position.x += dx * ratio;
            position.y += dy * ratio;
            position.altitude += dalt * ratio;
        }
    }
}

void Helicopter::updateFuel(double deltaTime) {
    double consumption = flightParams.fuelConsumption * (deltaTime / 60.0);
    
    // Increase consumption with damage and speed
    consumption *= (2.0 - systems.engineHealth); // Damaged engine uses more fuel
    consumption *= (1.0 + flightParams.speed / flightParams.maxSpeed); // Higher speed = more fuel
    
    flightParams.fuel -= consumption;
    if (flightParams.fuel < 0) flightParams.fuel = 0;
    
    if (flightParams.fuel <= 0 && systems.engine) {
        systems.engine = false;
        std::cout << "*** FUEL EXHAUSTED - ENGINE SHUTDOWN! ***" << std::endl;
    } else if (flightParams.fuel <= 50.0) {
        static bool lowFuelWarned = false;
        if (!lowFuelWarned) {
            std::cout << "*** LOW FUEL WARNING ***" << std::endl;
            lowFuelWarned = true;
        }
    }
}

double Helicopter::detectEnemy(const Enemy& enemy, WeatherCondition weather) const {
    if (!systems.radar) return 0.0;
    
    // Calculate distance to enemy
    double dx = enemy.getPosition().x - position.x;
    double dy = enemy.getPosition().y - position.y;
    double distance = sqrt(dx*dx + dy*dy);
    
    // Base detection probability
    double detectionChance = 1.0;
    
    // Range factor
    if (distance > radarRange) {
        detectionChance = 0.0;
    } else {
        detectionChance = 1.0 - (distance / radarRange);
    }
    
    // Weather effects
    detectionChance *= calculateWeatherEffect(weather);
    
    // System health effects
    detectionChance *= systems.radarHealth;
    
    // Enemy stealth factor (if applicable)
    if (enemy.getCapabilities().isAirborne) {
        detectionChance *= 0.7; // Airborne targets are harder to detect
    }
    
    return detectionChance;
}

double Helicopter::calculateWeatherEffect(WeatherCondition weather) const {
    switch (weather) {
        case WeatherCondition::CLEAR: return 1.0;
        case WeatherCondition::LIGHT_RAIN: return 0.9;
        case WeatherCondition::HEAVY_RAIN: return 0.6;
        case WeatherCondition::FOG: return 0.4;
        case WeatherCondition::SANDSTORM: return 0.3;
        default: return 1.0;
    }
}

void Helicopter::performRadarSweep(const std::vector<Enemy>& enemies, WeatherCondition weather) const {
    if (!systems.radar) {
        std::cout << "Radar system offline!" << std::endl;
        return;
    }
    
    std::cout << "\n=== RADAR SWEEP ===" << std::endl;
    std::cout << "Range: " << radarRange << "km" << std::endl;
    std::cout << "Weather effect: " << std::fixed << std::setprecision(1) 
              << calculateWeatherEffect(weather) * 100 << "%" << std::endl;
    
    int contactsDetected = 0;
    for (const auto& enemy : enemies) {
        double detectionChance = detectEnemy(enemy, weather);
        if (detectionChance > 0.5) { // 50% threshold for positive detection
            double dx = enemy.getPosition().x - position.x;
            double dy = enemy.getPosition().y - position.y;
            double distance = sqrt(dx*dx + dy*dy);
            double bearing = atan2(dy, dx) * 180.0 / M_PI;
            
            std::cout << "Contact: " << enemy.getType() 
                      << " at " << std::fixed << std::setprecision(1) << distance 
                      << "km, bearing " << std::fixed << std::setprecision(0) << bearing << "°" << std::endl;
            contactsDetected++;
        }
    }
    
    if (contactsDetected == 0) {
        std::cout << "No contacts detected." << std::endl;
    }
}

void Helicopter::showStatus() const {
    std::cout << "\n=== HELICOPTER STATUS ===" << std::endl;
    std::cout << "Aircraft: " << name << std::endl;
    std::cout << "Hull Integrity: " << std::fixed << std::setprecision(1) << health << "%" << std::endl;
    std::cout << "Position: (" << std::fixed << std::setprecision(1) 
              << position.x << ", " << position.y << ") at " << position.altitude << "m" << std::endl;
    std::cout << "Fuel: " << std::fixed << std::setprecision(0) 
              << flightParams.fuel << "/" << flightParams.maxFuel << " liters" << std::endl;
    std::cout << "Speed: " << std::fixed << std::setprecision(0) << flightParams.speed << " km/h" << std::endl;
    
    std::cout << "\nWeapons:" << std::endl;
    for (size_t i = 0; i < weapons.size(); ++i) {
        std::cout << "  " << (i+1) << ". " << weapons[i].getName() 
                  << " (" << weapons[i].getAmmoCount() << " rounds)" << std::endl;
    }
}

void Helicopter::showDetailedStatus() const {
    showStatus();
    showSystemsStatus();
}

void Helicopter::showSystemsStatus() const {
    std::cout << "\n=== SYSTEMS STATUS ===" << std::endl;
    std::cout << "Engine: " << (systems.engine ? "ONLINE" : "OFFLINE") 
              << " (" << std::fixed << std::setprecision(0) << systems.engineHealth * 100 << "%)" << std::endl;
    std::cout << "Main Rotor: " << (systems.mainRotor ? "ONLINE" : "OFFLINE")
              << " (" << std::fixed << std::setprecision(0) << systems.rotorHealth * 100 << "%)" << std::endl;
    std::cout << "Tail Rotor: " << (systems.tailRotor ? "ONLINE" : "OFFLINE") << std::endl;
    std::cout << "Radar: " << (systems.radar ? "ONLINE" : "OFFLINE")
              << " (" << std::fixed << std::setprecision(0) << systems.radarHealth * 100 << "%)" << std::endl;
    std::cout << "Navigation: " << (systems.navigation ? "ONLINE" : "OFFLINE") << std::endl;
    std::cout << "Weapons: " << (systems.weaponSystems ? "ONLINE" : "OFFLINE") << std::endl;
    std::cout << "Maneuverability: " << std::fixed << std::setprecision(0) 
              << flightParams.maneuverability * 100 << "%" << std::endl;
}

void Helicopter::addWeapon(const Weapon& weapon) {
    weapons.push_back(weapon);
}

void Helicopter::listWeapons() const {
    for (size_t i = 0; i < weapons.size(); i++) {
        std::cout << (i + 1) << ". " << weapons[i].getName() 
                  << " (" << weapons[i].getAmmoCount() << " rounds)";
        if (!weapons[i].hasAmmo()) {
            std::cout << " [NO AMMO]";
        }
        if (weapons[i].isReloading()) {
            std::cout << " [RELOADING]";
        }
        std::cout << std::endl;
    }
}

int Helicopter::getWeaponCount() const {
    return static_cast<int>(weapons.size());
}

bool Helicopter::hasAmmo(int weaponIndex) const {
    if (weaponIndex < 0 || weaponIndex >= static_cast<int>(weapons.size())) {
        return false;
    }
    return weapons[weaponIndex].hasAmmo();
}

void Helicopter::setDestination(const Position& dest) {
    destination = dest;
    isHovering = false;
}

void Helicopter::hover() {
    isHovering = true;
    flightParams.speed = 0.0;
}

void Helicopter::emergency_landing() {
    std::cout << "*** INITIATING EMERGENCY LANDING ***" << std::endl;
    destination.altitude = 0;
    isHovering = false;
    flightParams.speed = 50.0; // Slow descent
}

void Helicopter::refuel() {
    if (position.altitude <= 10.0) { // Must be on ground or very low
        flightParams.fuel = flightParams.maxFuel;
        std::cout << "Refueling complete. Fuel: " << flightParams.maxFuel << " liters" << std::endl;
    } else {
        std::cout << "Must land to refuel!" << std::endl;
    }
}

void Helicopter::performSystemCheck() const {
    std::cout << "\n=== SYSTEM CHECK ===" << std::endl;
    
    bool allSystemsGo = true;
    
    if (!systems.engine || systems.engineHealth < 0.5) {
        std::cout << "⚠️  ENGINE: Issues detected" << std::endl;
        allSystemsGo = false;
    } else {
        std::cout << "✅ ENGINE: Operational" << std::endl;
    }
    
    if (!systems.mainRotor || systems.rotorHealth < 0.5) {
        std::cout << "⚠️  ROTOR: Issues detected" << std::endl;
        allSystemsGo = false;
    } else {
        std::cout << "✅ ROTOR: Operational" << std::endl;
    }
    
    if (!systems.radar || systems.radarHealth < 0.5) {
        std::cout << "⚠️  RADAR: Issues detected" << std::endl;
        allSystemsGo = false;
    } else {
        std::cout << "✅ RADAR: Operational" << std::endl;
    }
    
    if (flightParams.fuel < 100.0) {
        std::cout << "⚠️  FUEL: Low" << std::endl;
        allSystemsGo = false;
    } else {
        std::cout << "✅ FUEL: Adequate" << std::endl;
    }
    
    if (allSystemsGo) {
        std::cout << "\n✅ ALL SYSTEMS OPERATIONAL - READY FOR MISSION" << std::endl;
    } else {
        std::cout << "\n⚠️  AIRCRAFT NOT READY - MAINTENANCE REQUIRED" << std::endl;
    }
}

void Helicopter::attackRandomEnemy(std::vector<Enemy>& enemies) {
    if (enemies.empty()) {
        std::cout << "No enemies left to attack!\n";
        return;
    }

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> dist(0, static_cast<int>(enemies.size()) - 1);
    int randomIndex = dist(rng);

    attack(enemies[randomIndex]);

    // Remove defeated enemy
    if (enemies[randomIndex].getHealth() <= 0) {
        std::cout << enemies[randomIndex].getType() << " was destroyed!" << std::endl;
        enemies.erase(enemies.begin() + randomIndex);
    }
}
