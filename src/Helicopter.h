#pragma once
#include <string>
#include <vector>
#include <map>
#include "Weapon.h"
#include "Enemy.h"

enum class WeatherCondition {
    CLEAR,
    LIGHT_RAIN,
    HEAVY_RAIN,
    FOG,
    SANDSTORM
};

enum class TerrainType {
    DESERT,
    FOREST,
    URBAN,
    MOUNTAIN,
    COASTAL
};

struct Position {
    double x, y, altitude;
    Position(double x = 0, double y = 0, double alt = 100) : x(x), y(y), altitude(alt) {}
};

struct FlightParams {
    double speed;           // km/h
    double maxSpeed;        // km/h
    double fuel;            // liters
    double maxFuel;         // liters
    double fuelConsumption; // liters per minute
    double maneuverability; // 0.0 to 1.0
};

struct HelicopterSystems {
    bool engine;
    bool mainRotor;
    bool tailRotor;
    bool radar;
    bool navigation;
    bool weaponSystems;
    double engineHealth;    // 0.0 to 1.0
    double rotorHealth;     // 0.0 to 1.0
    double radarHealth;     // 0.0 to 1.0
};

class Helicopter {
public:
    Helicopter(const std::string& name);
    Helicopter();
    
    // Combat methods
    void attack(Enemy& target);
    void attackRandomEnemy(std::vector<Enemy>& enemies);
    bool attackWithWeapon(Enemy& target, int weaponIndex, double distance);
    void takeDamage(int damage, const std::string& component = "hull");
    bool isAlive() const;
    bool canFly() const;
    
    // Weapon management
    void addWeapon(const Weapon& weapon);
    void listWeapons() const;
    int getWeaponCount() const;
    bool hasAmmo(int weaponIndex) const;
    
    // Flight operations
    void updatePosition(double deltaTime);
    void setDestination(const Position& dest);
    void hover();
    void emergency_landing();
    double calculateFuelConsumption(double deltaTime) const;
    void refuel();
    
    // Detection and radar
    double detectEnemy(const Enemy& enemy, WeatherCondition weather) const;
    bool isDetectedBy(const Enemy& enemy, double distance) const;
    void performRadarSweep(const std::vector<Enemy>& enemies, WeatherCondition weather) const;
    
    // Status and diagnostics
    void showStatus() const;
    void showDetailedStatus() const;
    void showSystemsStatus() const;
    void performSystemCheck() const;
    
    // Getters
    const Position& getPosition() const { return position; }
    const FlightParams& getFlightParams() const { return flightParams; }
    const HelicopterSystems& getSystems() const { return systems; }
    double getHealth() const { return health; }
    std::string getName() const { return name; }

private:
    // Core attributes
    std::string name;
    std::vector<Weapon> weapons;
    double health;          // 0.0 to 100.0
    Position position;
    Position destination;
    FlightParams flightParams;
    HelicopterSystems systems;
    
    // Radar and detection
    double radarRange;      // km
    double visualRange;     // km
    double stealthFactor;   // 0.0 to 1.0 (lower = stealthier)
    
    // Mission parameters
    bool isHovering;
    double timeInMission;   // minutes
    
    // Private helper methods
    void updateFuel(double deltaTime);
    void updateSystems();
    double calculateWeatherEffect(WeatherCondition weather) const;
    void damageSystem(const std::string& component, double damage);
};
