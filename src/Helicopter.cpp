#include <iostream>
#include <random>
#include <iomanip>
#include <cmath>

// Define M_PI for Windows compatibility
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "Helicopter.h"

Helicopter::Helicopter(const std::string& name) 
    : name(name), health(100.0), position(0, 0, 100), 
      radarRange(25.0), visualRange(5.0), stealthFactor(0.3),
      isHovering(false), timeInMission(0.0), defensiveMode(false),
      flareCount(30), chaffCount(20), evasionBonus(0.0) {
    
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
    systems.armorRating = 0.25; // 25% damage reduction
    
    // Initialize default weapons loadout
    initializeDefaultWeapons();
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
    // Apply armor reduction first
    double actualDamage = damage * (1.0 - systems.armorRating);
    
    std::cout << name << " armor reduces " << damage << " damage to " 
              << std::fixed << std::setprecision(1) << actualDamage << std::endl;
    
    // Apply damage to specific component
    if (component == "engine") {
        systems.engineHealth -= actualDamage / 200.0;
        if (systems.engineHealth <= 0) {
            systems.engine = false;
            systems.engineHealth = 0;
            std::cout << "*** ENGINE CRITICAL DAMAGE! ***" << std::endl;
        }
    } else if (component == "rotor") {
        systems.rotorHealth -= actualDamage / 150.0;
        if (systems.rotorHealth <= 0.3) {
            flightParams.maneuverability *= 0.5;
            std::cout << "*** ROTOR DAMAGE - Reduced maneuverability! ***" << std::endl;
        }
    } else if (component == "radar") {
        systems.radarHealth -= actualDamage / 100.0;
        if (systems.radarHealth <= 0) {
            systems.radar = false;
            systems.radarHealth = 0;
            std::cout << "*** RADAR SYSTEM DESTROYED! ***" << std::endl;
        }
    }
    
    // Apply general hull damage
    health -= actualDamage;
    if (health < 0) health = 0;
    
    std::cout << name << " takes " << std::fixed << std::setprecision(1) << actualDamage 
              << " damage to " << component 
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
    
    // Update position based on current speed
    if (flightParams.speed > 0 && destination.x != position.x && destination.y != position.y) {
        double dx = destination.x - position.x;
        double dy = destination.y - position.y;
        double distance = sqrt(dx*dx + dy*dy);
        
        if (distance > 0.1) { // 100m threshold
            double moveDistance = (flightParams.speed / 3600.0) * deltaTime; // km
            double ratio = std::min(1.0, moveDistance / distance);
            
            position.x += dx * ratio;
            position.y += dy * ratio;
        }
    }
}

// Utility methods for navigation
double Helicopter::calculateDistance(const EnemyPosition& enemyPos) const {
    double dx = enemyPos.x - position.x;
    double dy = enemyPos.y - position.y;
    return std::sqrt(dx * dx + dy * dy);
}

double Helicopter::calculateBearing(const EnemyPosition& enemyPos) const {
    double dx = enemyPos.x - position.x;
    double dy = enemyPos.y - position.y;
    double bearing = std::atan2(dy, dx) * 180.0 / M_PI;
    // Convert to 0-360 range
    if (bearing < 0) bearing += 360.0;
    return bearing;
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
                      << "km, bearing " << std::fixed << std::setprecision(0) << bearing << " deg" << std::endl;
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
        std::cout << "[!] ENGINE: Issues detected" << std::endl;
        allSystemsGo = false;
    } else {
        std::cout << "[OK] ENGINE: Operational" << std::endl;
    }
    
    if (!systems.mainRotor || systems.rotorHealth < 0.5) {
        std::cout << "[!] ROTOR: Issues detected" << std::endl;
        allSystemsGo = false;
    } else {
        std::cout << "[OK] ROTOR: Operational" << std::endl;
    }
    
    if (!systems.radar || systems.radarHealth < 0.5) {
        std::cout << "[!] RADAR: Issues detected" << std::endl;
        allSystemsGo = false;
    } else {
        std::cout << "[OK] RADAR: Operational" << std::endl;
    }
    
    if (flightParams.fuel < 100.0) {
        std::cout << "[!] FUEL: Low" << std::endl;
        allSystemsGo = false;
    } else {
        std::cout << "[OK] FUEL: Adequate" << std::endl;
    }
    
    if (allSystemsGo) {
        std::cout << "\n[OK] ALL SYSTEMS OPERATIONAL - READY FOR MISSION" << std::endl;
    } else {
        std::cout << "\n[!] AIRCRAFT NOT READY - MAINTENANCE REQUIRED" << std::endl;
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

void Helicopter::performEvasiveManeuvers() {
    if (!canFly() || !systems.mainRotor) {
        std::cout << "Cannot perform evasive maneuvers - flight systems compromised!" << std::endl;
        return;
    }
    
    defensiveMode = true;
    evasionBonus = 0.4 * flightParams.maneuverability; // Up to 40% evasion bonus
    
    // Evasive maneuvers consume extra fuel
    flightParams.fuel -= 10.0;
    
    std::cout << name << " performs evasive maneuvers! Evasion increased by " 
              << std::fixed << std::setprecision(0) << evasionBonus * 100 << "%!" << std::endl;
}

void Helicopter::deployCountermeasures() {
    if (flareCount > 0 && chaffCount > 0) {
        popFlares();
        dropChaff();
        std::cout << name << " deploys full countermeasure suite!" << std::endl;
    } else if (flareCount > 0) {
        popFlares();
    } else if (chaffCount > 0) {
        dropChaff();
    } else {
        std::cout << "No countermeasures remaining!" << std::endl;
    }
}

void Helicopter::popFlares() {
    if (flareCount > 0) {
        flareCount--;
        evasionBonus += 0.3; // 30% bonus against heat-seeking missiles
        std::cout << name << " pops flares! Heat-seeking missile defense activated. (" 
                  << flareCount << " flares remaining)" << std::endl;
    } else {
        std::cout << "No flares remaining!" << std::endl;
    }
}

void Helicopter::dropChaff() {
    if (chaffCount > 0) {
        chaffCount--;
        evasionBonus += 0.25; // 25% bonus against radar-guided missiles
        std::cout << name << " drops chaff! Radar-guided missile defense activated. (" 
                  << chaffCount << " chaff remaining)" << std::endl;
    } else {
        std::cout << "No chaff remaining!" << std::endl;
    }
}

bool Helicopter::isInDefensiveMode() const {
    return defensiveMode;
}

void Helicopter::setDefensiveMode(bool active) {
    defensiveMode = active;
    if (!active) {
        evasionBonus = 0.0;
        std::cout << name << " exits defensive mode." << std::endl;
    }
}

double Helicopter::getEvasionBonus() const {
    return evasionBonus;
}

void Helicopter::showTacticalAdvice() const {
    std::cout << "\n=== TACTICAL COMBAT GUIDE ===" << std::endl;
    std::cout << "HELICOPTER ADVANTAGES:" << std::endl;
    std::cout << "- Mobility - Use speed and altitude to stay out of range" << std::endl;
    std::cout << "- Standoff weapons - Attack from maximum weapon range" << std::endl;
    std::cout << "- Terrain masking - Use hills/buildings to break line of sight" << std::endl;
    std::cout << "- Hit and run - Attack quickly, then retreat and reposition" << std::endl;
    
    std::cout << "\nCOUNTER-TACTICS BY ENEMY:" << std::endl;
    std::cout << "LIGHT TANKS (Range: 4.5km, Damage: 15-25):" << std::endl;
    std::cout << "- Attack from >5km with missiles" << std::endl;
    std::cout << "- Use evasive maneuvers when closing distance" << std::endl;
    std::cout << "- Deploy flares/chaff if they lock on" << std::endl;
    
    std::cout << "\nSAM SITES (Range: 8km, Damage: 25-45):" << std::endl;
    std::cout << "- Stay at maximum range (8km+) when possible" << std::endl;
    std::cout << "- Use guided missiles for precision strikes" << std::endl;
    std::cout << "- Pop countermeasures immediately if detected" << std::endl;
    std::cout << "- Attack from different angles to confuse targeting" << std::endl;
    
    std::cout << "\nDEFENSIVE SYSTEMS:" << std::endl;
    std::cout << "- Armor: " << std::fixed << std::setprecision(0) << systems.armorRating * 100 << "% damage reduction" << std::endl;
    std::cout << "- Flares: " << flareCount << " remaining (anti-heat seeking)" << std::endl;
    std::cout << "- Chaff: " << chaffCount << " remaining (anti-radar)" << std::endl;
    std::cout << "- Evasion bonus: " << std::fixed << std::setprecision(0) << evasionBonus * 100 << "%" << std::endl;
}

void Helicopter::initializeDefaultWeapons() {
    // Clear any existing weapons
    weapons.clear();
    
    // Add default weapon loadout for combat helicopter
    WeaponSpecs chainGunSpecs;
    chainGunSpecs.range = 3.0;
    chainGunSpecs.accuracy = 0.85;
    chainGunSpecs.reloadTime = 1.0;
    chainGunSpecs.penetration = 25.0;
    chainGunSpecs.requiresLOS = true;
    chainGunSpecs.blastRadius = 0.0;
    chainGunSpecs.lockOnTime = 0.0;
    
    WeaponSpecs missileSpecs;
    missileSpecs.range = 8.0;
    missileSpecs.accuracy = 0.9;
    missileSpecs.reloadTime = 3.0;
    missileSpecs.penetration = 80.0;
    missileSpecs.requiresLOS = true;
    missileSpecs.blastRadius = 10.0;
    missileSpecs.lockOnTime = 2.0;
    
    WeaponSpecs rocketSpecs;
    rocketSpecs.range = 5.0;
    rocketSpecs.accuracy = 0.75;
    rocketSpecs.reloadTime = 2.0;
    rocketSpecs.penetration = 50.0;
    rocketSpecs.requiresLOS = true;
    rocketSpecs.blastRadius = 15.0;
    rocketSpecs.lockOnTime = 0.0;
    
    // Air-to-air missile specs (Sidewinder-type)
    WeaponSpecs aaSpecs;
    aaSpecs.range = 15.0;
    aaSpecs.accuracy = 0.88;
    aaSpecs.reloadTime = 4.0;
    aaSpecs.penetration = 60.0;
    aaSpecs.requiresLOS = true;
    aaSpecs.blastRadius = 8.0;
    aaSpecs.lockOnTime = 3.0;
    
    // Guided missile specs (TOW-type)
    WeaponSpecs guidedSpecs;
    guidedSpecs.range = 12.0;
    guidedSpecs.accuracy = 0.95;
    guidedSpecs.reloadTime = 5.0;
    guidedSpecs.penetration = 90.0;
    guidedSpecs.requiresLOS = true;
    guidedSpecs.blastRadius = 12.0;
    guidedSpecs.lockOnTime = 4.0;
    
    // Machine gun specs
    WeaponSpecs mgSpecs;
    mgSpecs.range = 2.0;
    mgSpecs.accuracy = 0.8;
    mgSpecs.reloadTime = 0.5;
    mgSpecs.penetration = 15.0;
    mgSpecs.requiresLOS = true;
    mgSpecs.blastRadius = 0.0;
    mgSpecs.lockOnTime = 0.0;
    
    // Add weapons to helicopter
    weapons.emplace_back("M230 Chain Gun", WeaponType::CANNON, 15, 25, 1200, chainGunSpecs);
    weapons.emplace_back("Hellfire Missile", WeaponType::AIR_TO_GROUND_MISSILE, 80, 120, 8, missileSpecs);
    weapons.emplace_back("Hydra 70 Rockets", WeaponType::ROCKET_POD, 35, 55, 38, rocketSpecs);
    weapons.emplace_back("AIM-9 Sidewinder", WeaponType::AIR_TO_AIR_MISSILE, 55, 85, 4, aaSpecs);
    weapons.emplace_back("TOW Missile", WeaponType::GUIDED_MISSILE, 90, 130, 6, guidedSpecs);
    weapons.emplace_back("M134 Minigun", WeaponType::MACHINE_GUN, 8, 15, 2000, mgSpecs);
    
    // Removed automatic weapon display - let user choose when to see weapons
}
