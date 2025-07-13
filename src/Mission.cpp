#include "Mission.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>

Mission::Mission(MissionType type, const std::string& name, const std::string& briefing)
    : missionType(type), status(MissionStatus::NOT_STARTED), missionName(name), 
      briefing(briefing), startTime(0.0), elapsedTime(0.0), enemiesDestroyed(0),
      damageTaken(0), stealthMaintained(true), accuracyRating(1.0) {
    
    generateObjectives();
}

void Mission::generateObjectives() {
    objectives.clear();
    
    switch (missionType) {
        case MissionType::SEARCH_AND_DESTROY:
            objectives.emplace_back("Destroy all enemy units in the area", true, 10);
            objectives.emplace_back("Maintain stealth until first engagement", false, 7);
            objectives.emplace_back("Return to base safely", true, 9);
            break;
            
        case MissionType::RECONNAISSANCE:
            objectives.emplace_back("Identify enemy positions", true, 10);
            objectives.emplace_back("Maintain stealth throughout mission", true, 8);
            objectives.emplace_back("Take no damage", false, 6);
            objectives.emplace_back("Complete mission within time limit", true, 7);
            break;
            
        case MissionType::ESCORT:
            objectives.emplace_back("Protect convoy until destination", true, 10);
            objectives.emplace_back("Eliminate air threats", true, 9);
            objectives.emplace_back("Minimize convoy casualties", false, 8);
            break;
            
        case MissionType::RESCUE:
            objectives.emplace_back("Locate and extract personnel", true, 10);
            objectives.emplace_back("Clear landing zone", true, 9);
            objectives.emplace_back("Complete rescue within time limit", true, 8);
            break;
            
        case MissionType::GROUND_SUPPORT:
            objectives.emplace_back("Support ground forces", true, 10);
            objectives.emplace_back("Eliminate priority targets", true, 9);
            objectives.emplace_back("Respond to danger close calls", false, 7);
            break;
            
        case MissionType::PATROL:
            objectives.emplace_back("Complete patrol route", true, 8);
            objectives.emplace_back("Report enemy activity", false, 6);
            objectives.emplace_back("Maintain patrol schedule", false, 5);
            break;
            
        default:
            objectives.emplace_back("Complete primary objective", true, 10);
            break;
    }
}

void Mission::start() {
    status = MissionStatus::IN_PROGRESS;
    startTime = 0.0;
    elapsedTime = 0.0;
    std::cout << "\n=== MISSION START ===" << std::endl;
    std::cout << "Mission: " << missionName << std::endl;
    showDetailedBriefing();
}

void Mission::update(double deltaTime) {
    if (status != MissionStatus::IN_PROGRESS) return;
    
    elapsedTime += deltaTime;
    updateProgress(deltaTime);
    
    // Check failure conditions
    if (checkFailureConditions()) {
        fail("Critical objectives failed");
    }
    
    // Check completion
    if (areAllObjectivesComplete()) {
        complete();
    }
    
    // Check time limit
    if (parameters.timeLimit > 0 && elapsedTime >= parameters.timeLimit) {
        fail("Time limit exceeded");
    }
}

void Mission::updateProgress(double deltaTime) {
    // Update mission-specific logic here
    // This could include checking for stealth breaks, time constraints, etc.
}

bool Mission::checkFailureConditions() const {
    return hasCriticalObjectiveFailed();
}

void Mission::complete() {
    if (status == MissionStatus::IN_PROGRESS) {
        status = MissionStatus::COMPLETED;
        std::cout << "\n=== MISSION COMPLETE ===" << std::endl;
        std::cout << "All objectives achieved!" << std::endl;
        std::cout << "Mission Rating: " << getMissionRating() << std::endl;
        std::cout << "Score: " << calculateScore() << std::endl;
    }
}

void Mission::fail(const std::string& reason) {
    if (status == MissionStatus::IN_PROGRESS) {
        status = MissionStatus::FAILED;
        std::cout << "\n=== MISSION FAILED ===" << std::endl;
        if (!reason.empty()) {
            std::cout << "Reason: " << reason << std::endl;
        }
        std::cout << "Score: " << calculateScore() << std::endl;
    }
}

void Mission::abort() {
    status = MissionStatus::ABORTED;
    std::cout << "\n=== MISSION ABORTED ===" << std::endl;
    std::cout << "Mission terminated by pilot decision." << std::endl;
}

void Mission::addObjective(const Objective& objective) {
    objectives.push_back(objective);
}

void Mission::completeObjective(size_t index) {
    if (index < objectives.size()) {
        objectives[index].completed = true;
        std::cout << "✅ Objective completed: " << objectives[index].description << std::endl;
    }
}

bool Mission::areAllObjectivesComplete() const {
    for (const auto& obj : objectives) {
        if (!obj.completed && obj.critical) {
            return false;
        }
    }
    return true;
}

bool Mission::hasCriticalObjectiveFailed() const {
    // For now, we assume critical objectives only fail through external conditions
    return false;
}

double Mission::getProgress() const {
    if (objectives.empty()) return 0.0;
    
    int completedObjectives = 0;
    int totalWeight = 0;
    int completedWeight = 0;
    
    for (const auto& obj : objectives) {
        totalWeight += obj.priority;
        if (obj.completed) {
            completedObjectives++;
            completedWeight += obj.priority;
        }
    }
    
    return static_cast<double>(completedWeight) / totalWeight;
}

double Mission::getTimeRemaining() const {
    if (parameters.timeLimit <= 0) return -1.0; // No time limit
    return std::max(0.0, parameters.timeLimit - elapsedTime);
}

void Mission::showMissionStatus() const {
    std::cout << "\n=== MISSION STATUS ===" << std::endl;
    std::cout << "Mission: " << missionName << std::endl;
    std::cout << "Status: ";
    switch (status) {
        case MissionStatus::NOT_STARTED: std::cout << "NOT STARTED"; break;
        case MissionStatus::IN_PROGRESS: std::cout << "IN PROGRESS"; break;
        case MissionStatus::COMPLETED: std::cout << "COMPLETED"; break;
        case MissionStatus::FAILED: std::cout << "FAILED"; break;
        case MissionStatus::ABORTED: std::cout << "ABORTED"; break;
    }
    std::cout << std::endl;
    
    std::cout << "Progress: " << std::fixed << std::setprecision(1) 
              << getProgress() * 100 << "%" << std::endl;
    
    if (parameters.timeLimit > 0) {
        std::cout << "Time remaining: " << std::fixed << std::setprecision(1) 
                  << getTimeRemaining() << " minutes" << std::endl;
    }
    
    std::cout << "\nObjectives:" << std::endl;
    for (size_t i = 0; i < objectives.size(); ++i) {
        std::cout << "  " << (objectives[i].completed ? "✅" : "⏳") 
                  << " " << objectives[i].description;
        if (objectives[i].critical) {
            std::cout << " (CRITICAL)";
        }
        std::cout << std::endl;
    }
}

void Mission::showDetailedBriefing() const {
    std::cout << "\n=== MISSION BRIEFING ===" << std::endl;
    std::cout << "Operation: " << missionName << std::endl;
    std::cout << "Type: ";
    switch (missionType) {
        case MissionType::SEARCH_AND_DESTROY: std::cout << "Search & Destroy"; break;
        case MissionType::RECONNAISSANCE: std::cout << "Reconnaissance"; break;
        case MissionType::ESCORT: std::cout << "Escort"; break;
        case MissionType::RESCUE: std::cout << "Rescue"; break;
        case MissionType::SUPPLY_DROP: std::cout << "Supply Drop"; break;
        case MissionType::GROUND_SUPPORT: std::cout << "Close Air Support"; break;
        case MissionType::PATROL: std::cout << "Combat Air Patrol"; break;
    }
    std::cout << std::endl;
    
    std::cout << "\nMission Description:" << std::endl;
    std::cout << briefing << std::endl;
    
    std::cout << "\nOperational Area: " << parameters.area << std::endl;
    std::cout << "Terrain: ";
    switch (parameters.terrain) {
        case TerrainType::DESERT: std::cout << "Desert"; break;
        case TerrainType::FOREST: std::cout << "Forest"; break;
        case TerrainType::URBAN: std::cout << "Urban"; break;
        case TerrainType::MOUNTAIN: std::cout << "Mountain"; break;
        case TerrainType::COASTAL: std::cout << "Coastal"; break;
    }
    std::cout << std::endl;
    
    std::cout << "Weather: ";
    switch (parameters.weather) {
        case WeatherCondition::CLEAR: std::cout << "Clear"; break;
        case WeatherCondition::LIGHT_RAIN: std::cout << "Light Rain"; break;
        case WeatherCondition::HEAVY_RAIN: std::cout << "Heavy Rain"; break;
        case WeatherCondition::FOG: std::cout << "Fog"; break;
        case WeatherCondition::SANDSTORM: std::cout << "Sandstorm"; break;
    }
    std::cout << std::endl;
    
    if (parameters.timeLimit > 0) {
        std::cout << "Time Limit: " << std::fixed << std::setprecision(0) 
                  << parameters.timeLimit << " minutes" << std::endl;
    }
    
    if (parameters.stealthRequired) {
        std::cout << "STEALTH REQUIRED - Avoid detection" << std::endl;
    }
    
    std::cout << "\nObjectives:" << std::endl;
    for (size_t i = 0; i < objectives.size(); ++i) {
        std::cout << "  " << (i + 1) << ". " << objectives[i].description;
        if (objectives[i].critical) {
            std::cout << " (CRITICAL)";
        }
        std::cout << std::endl;
    }
}

int Mission::calculateScore() const {
    int baseScore = 1000;
    
    // Objective completion bonus
    for (const auto& obj : objectives) {
        if (obj.completed) {
            baseScore += obj.priority * 100;
        } else if (obj.critical) {
            baseScore -= 500; // Heavy penalty for failed critical objectives
        }
    }
    
    // Time bonus (if completed within time limit)
    if (status == MissionStatus::COMPLETED && parameters.timeLimit > 0) {
        double timeBonus = (parameters.timeLimit - elapsedTime) / parameters.timeLimit;
        baseScore += static_cast<int>(timeBonus * 500);
    }
    
    // Stealth bonus
    if (stealthMaintained && parameters.stealthRequired) {
        baseScore += 300;
    }
    
    // Damage penalty
    baseScore -= damageTaken * 5;
    
    // Accuracy bonus
    baseScore += static_cast<int>(accuracyRating * 200);
    
    return std::max(0, baseScore);
}

std::string Mission::getMissionRating() const {
    int score = calculateScore();
    
    if (score >= 2000) return "EXCELLENT";
    else if (score >= 1500) return "GOOD";
    else if (score >= 1000) return "SATISFACTORY";
    else if (score >= 500) return "POOR";
    else return "FAILURE";
}

void Mission::addThreat(const Enemy& enemy) {
    threats.push_back(enemy);
}

void Mission::removeThreat(size_t index) {
    if (index < threats.size()) {
        threats.erase(threats.begin() + index);
    }
}

bool Mission::isInNoFlyZone(const Position& pos) const {
    for (const auto& zone : parameters.noFlyZones) {
        double distance = calculateDistance(pos, zone);
        if (distance < 2.0) { // 2km no-fly radius
            return true;
        }
    }
    return false;
}

bool Mission::isInThreatArea(const Position& pos) const {
    for (const auto& threat : parameters.threatAreas) {
        double distance = calculateDistance(pos, threat);
        if (distance < 5.0) { // 5km threat radius
            return true;
        }
    }
    return false;
}

double Mission::calculateDistance(const Position& pos1, const EnemyPosition& pos2) const {
    double dx = pos1.x - pos2.x;
    double dy = pos1.y - pos2.y;
    return sqrt(dx*dx + dy*dy);
}
