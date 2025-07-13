#pragma once
#include <random>
#include <vector>
#include "Mission.h"

class Environment {
public:
    Environment();
    
    // Weather system
    void updateWeather(double deltaTime);
    WeatherCondition getCurrentWeather() const { return currentWeather; }
    double getVisibilityModifier() const;
    double getWeaponAccuracyModifier() const;
    std::string getWeatherDescription() const;
    
    // Terrain system
    void setTerrain(TerrainType terrain) { currentTerrain = terrain; }
    TerrainType getCurrentTerrain() const { return currentTerrain; }
    double getTerrainCoverBonus(const Position& pos) const;
    double getMovementModifier() const;
    std::string getTerrainDescription() const;
    
    // Time and lighting
    void updateTimeOfDay(double deltaTime);
    double getTimeOfDay() const { return timeOfDay; } // 0.0 to 24.0
    bool isNightTime() const { return timeOfDay > 20.0 || timeOfDay < 6.0; }
    bool isDayTime() const { return timeOfDay >= 6.0 && timeOfDay <= 20.0; }
    double getLightingModifier() const;
    std::string getTimeDescription() const;
    
    // Environmental hazards
    bool hasRadarInterference() const;
    double getWindSpeed() const;
    double getWindDirection() const; // degrees
    bool hasDustStorm() const;
    bool hasElectronicWarfare() const;
    
    // Simulation control
    void setWeather(WeatherCondition weather) { currentWeather = weather; }
    void setTimeOfDay(double time) { timeOfDay = time; }
    void enableDynamicWeather(bool enable) { dynamicWeather = enable; }
    
    // Environmental effects on gameplay
    double calculateDetectionModifier() const;
    double calculateCommunicationReliability() const;
    std::vector<std::string> getEnvironmentalWarnings() const;
    
    // Status display
    void showEnvironmentalStatus() const;
    void showDetailedEnvironment() const;

private:
    // Weather system
    WeatherCondition currentWeather;
    double weatherDuration;        // minutes
    double weatherChangeTimer;
    bool dynamicWeather;
    double windSpeed;             // km/h
    double windDirection;         // degrees
    
    // Terrain
    TerrainType currentTerrain;
    
    // Time system
    double timeOfDay;             // 0.0 to 24.0 hours
    double dayDuration;           // minutes for full day cycle
    
    // Environmental conditions
    bool electronicWarfare;
    double radarJamming;          // 0.0 to 1.0
    bool dustStorm;
    
    // Random generation
    mutable std::mt19937 rng;
    
    // Private helper methods
    void randomizeWeather();
    WeatherCondition getRandomWeather() const;
    double calculateWeatherVisibility() const;
    double calculateTerrainVisibility() const;
};
