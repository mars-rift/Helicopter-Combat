#include "Environment.h"
#include <iostream>
#include <iomanip>
#include <sstream>

Environment::Environment() 
    : currentWeather(WeatherCondition::CLEAR), weatherDuration(60.0), 
      weatherChangeTimer(0.0), dynamicWeather(true), windSpeed(10.0), 
      windDirection(90.0), currentTerrain(TerrainType::DESERT), 
      timeOfDay(12.0), dayDuration(120.0), electronicWarfare(false), 
      radarJamming(0.0), dustStorm(false), rng(std::random_device{}()) {
}

void Environment::updateWeather(double deltaTime) {
    if (!dynamicWeather) return;
    
    weatherChangeTimer += deltaTime;
    
    if (weatherChangeTimer >= weatherDuration) {
        randomizeWeather();
        weatherChangeTimer = 0.0;
    }
    
    // Update wind
    std::uniform_real_distribution<double> windDist(-5.0, 5.0);
    windSpeed += windDist(rng) * deltaTime;
    windSpeed = std::max(0.0, std::min(50.0, windSpeed));
    
    windDirection += windDist(rng) * deltaTime * 10.0;
    if (windDirection >= 360.0) windDirection -= 360.0;
    if (windDirection < 0.0) windDirection += 360.0;
}

void Environment::randomizeWeather() {
    std::uniform_int_distribution<int> weatherDist(0, 4);
    int weatherChoice = weatherDist(rng);
    
    WeatherCondition newWeather = static_cast<WeatherCondition>(weatherChoice);
    
    if (newWeather != currentWeather) {
        currentWeather = newWeather;
        std::cout << "Weather change: " << getWeatherDescription() << std::endl;
        
        // Set new weather duration
        std::uniform_real_distribution<double> durationDist(30.0, 120.0);
        weatherDuration = durationDist(rng);
    }
}

WeatherCondition Environment::getRandomWeather() const {
    std::uniform_int_distribution<int> dist(0, 4);
    return static_cast<WeatherCondition>(dist(rng));
}

double Environment::getVisibilityModifier() const {
    double weatherVis = calculateWeatherVisibility();
    double terrainVis = calculateTerrainVisibility();
    double lightingVis = getLightingModifier();
    
    return weatherVis * terrainVis * lightingVis;
}

double Environment::calculateWeatherVisibility() const {
    switch (currentWeather) {
        case WeatherCondition::CLEAR: return 1.0;
        case WeatherCondition::LIGHT_RAIN: return 0.8;
        case WeatherCondition::HEAVY_RAIN: return 0.5;
        case WeatherCondition::FOG: return 0.3;
        case WeatherCondition::SANDSTORM: return 0.2;
        default: return 1.0;
    }
}

double Environment::calculateTerrainVisibility() const {
    switch (currentTerrain) {
        case TerrainType::DESERT: return 1.0;
        case TerrainType::FOREST: return 0.7;
        case TerrainType::URBAN: return 0.8;
        case TerrainType::MOUNTAIN: return 0.9;
        case TerrainType::COASTAL: return 0.95;
        default: return 1.0;
    }
}

double Environment::getWeaponAccuracyModifier() const {
    double modifier = getVisibilityModifier();
    
    // Wind effect on accuracy
    if (windSpeed > 20.0) {
        modifier *= 0.9; // High winds reduce accuracy
    }
    
    // Electronic warfare effect
    if (electronicWarfare) {
        modifier *= 0.8;
    }
    
    return modifier;
}

std::string Environment::getWeatherDescription() const {
    switch (currentWeather) {
        case WeatherCondition::CLEAR: return "Clear skies";
        case WeatherCondition::LIGHT_RAIN: return "Light rain";
        case WeatherCondition::HEAVY_RAIN: return "Heavy rain";
        case WeatherCondition::FOG: return "Dense fog";
        case WeatherCondition::SANDSTORM: return "Sandstorm";
        default: return "Unknown";
    }
}

std::string Environment::getTerrainDescription() const {
    switch (currentTerrain) {
        case TerrainType::DESERT: return "Desert terrain - Open visibility, minimal cover";
        case TerrainType::FOREST: return "Forest terrain - Limited visibility, good cover";
        case TerrainType::URBAN: return "Urban terrain - Variable visibility, numerous obstacles";
        case TerrainType::MOUNTAIN: return "Mountain terrain - Altitude variations, rocky terrain";
        case TerrainType::COASTAL: return "Coastal terrain - Mixed land/water, variable conditions";
        default: return "Unknown terrain";
    }
}

void Environment::updateTimeOfDay(double deltaTime) {
    timeOfDay += (deltaTime / 60.0) * (24.0 / dayDuration); // Convert to hours
    
    if (timeOfDay >= 24.0) {
        timeOfDay -= 24.0;
    }
}

double Environment::getLightingModifier() const {
    if (isDayTime()) {
        return 1.0;
    } else if (isNightTime()) {
        return 0.6; // Reduced visibility at night
    } else {
        // Dawn/dusk
        return 0.8;
    }
}

std::string Environment::getTimeDescription() const {
    int hours = static_cast<int>(timeOfDay);
    int minutes = static_cast<int>((timeOfDay - hours) * 60);
    
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << hours << ":" 
       << std::setfill('0') << std::setw(2) << minutes;
    
    if (isNightTime()) {
        return ss.str() + " (Night)";
    } else if (isDayTime()) {
        return ss.str() + " (Day)";
    } else {
        return ss.str() + " (Dawn/Dusk)";
    }
}

bool Environment::hasRadarInterference() const {
    return electronicWarfare || (currentWeather == WeatherCondition::HEAVY_RAIN) ||
           (currentWeather == WeatherCondition::SANDSTORM);
}

bool Environment::hasDustStorm() const {
    return dustStorm || (currentWeather == WeatherCondition::SANDSTORM);
}

bool Environment::hasElectronicWarfare() const {
    return electronicWarfare;
}

double Environment::getTerrainCoverBonus(const Position& pos) const {
    // Simple terrain cover calculation based on terrain type
    switch (currentTerrain) {
        case TerrainType::FOREST: return 0.3;  // 30% cover bonus
        case TerrainType::URBAN: return 0.4;   // 40% cover bonus
        case TerrainType::MOUNTAIN: return 0.2; // 20% cover bonus
        case TerrainType::DESERT: return 0.0;  // No cover
        case TerrainType::COASTAL: return 0.1; // Minimal cover
        default: return 0.0;
    }
}

double Environment::getMovementModifier() const {
    double modifier = 1.0;
    
    // Weather effects on movement
    switch (currentWeather) {
        case WeatherCondition::HEAVY_RAIN:
        case WeatherCondition::SANDSTORM:
            modifier *= 0.7;
            break;
        case WeatherCondition::FOG:
        case WeatherCondition::LIGHT_RAIN:
            modifier *= 0.9;
            break;
        default:
            break;
    }
    
    // Terrain effects on movement
    switch (currentTerrain) {
        case TerrainType::MOUNTAIN:
            modifier *= 0.8;
            break;
        case TerrainType::FOREST:
        case TerrainType::URBAN:
            modifier *= 0.9;
            break;
        default:
            break;
    }
    
    return modifier;
}

double Environment::calculateDetectionModifier() const {
    return getVisibilityModifier() * (1.0 - radarJamming);
}

double Environment::calculateCommunicationReliability() const {
    double reliability = 1.0;
    
    if (electronicWarfare) {
        reliability *= 0.6;
    }
    
    if (hasRadarInterference()) {
        reliability *= 0.8;
    }
    
    if (currentTerrain == TerrainType::MOUNTAIN) {
        reliability *= 0.9; // Mountains can block communications
    }
    
    return reliability;
}

std::vector<std::string> Environment::getEnvironmentalWarnings() const {
    std::vector<std::string> warnings;
    
    if (getVisibilityModifier() < 0.5) {
        warnings.push_back("Low visibility conditions");
    }
    
    if (windSpeed > 25.0) {
        warnings.push_back("High wind speeds affecting accuracy");
    }
    
    if (hasRadarInterference()) {
        warnings.push_back("Radar interference detected");
    }
    
    if (electronicWarfare) {
        warnings.push_back("Electronic warfare environment");
    }
    
    if (isNightTime()) {
        warnings.push_back("Night operations - reduced visibility");
    }
    
    return warnings;
}

void Environment::showEnvironmentalStatus() const {
    std::cout << "\n=== ENVIRONMENTAL STATUS ===" << std::endl;
    std::cout << "Time: " << getTimeDescription() << std::endl;
    std::cout << "Weather: " << getWeatherDescription() << std::endl;
    std::cout << "Terrain: ";
    switch (currentTerrain) {
        case TerrainType::DESERT: std::cout << "Desert"; break;
        case TerrainType::FOREST: std::cout << "Forest"; break;
        case TerrainType::URBAN: std::cout << "Urban"; break;
        case TerrainType::MOUNTAIN: std::cout << "Mountain"; break;
        case TerrainType::COASTAL: std::cout << "Coastal"; break;
    }
    std::cout << std::endl;
    
    std::cout << "Visibility: " << std::fixed << std::setprecision(0) 
              << getVisibilityModifier() * 100 << "%" << std::endl;
    std::cout << "Wind: " << std::fixed << std::setprecision(0) 
              << windSpeed << " km/h from " << windDirection << " deg" << std::endl;
    
    auto warnings = getEnvironmentalWarnings();
    if (!warnings.empty()) {
        std::cout << "\nWarnings:" << std::endl;
        for (const auto& warning : warnings) {
            std::cout << "[!] " << warning << std::endl;
        }
    }
}

void Environment::showDetailedEnvironment() const {
    showEnvironmentalStatus();
    
    std::cout << "\nDetailed Conditions:" << std::endl;
    std::cout << "Movement modifier: " << std::fixed << std::setprecision(0) 
              << getMovementModifier() * 100 << "%" << std::endl;
    std::cout << "Weapon accuracy modifier: " << std::fixed << std::setprecision(0) 
              << getWeaponAccuracyModifier() * 100 << "%" << std::endl;
    std::cout << "Detection modifier: " << std::fixed << std::setprecision(0) 
              << calculateDetectionModifier() * 100 << "%" << std::endl;
    std::cout << "Communication reliability: " << std::fixed << std::setprecision(0) 
              << calculateCommunicationReliability() * 100 << "%" << std::endl;
    
    std::cout << "\n" << getTerrainDescription() << std::endl;
}
