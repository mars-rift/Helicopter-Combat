#include "Game.h"
#include <iostream>
#include <random>
#include <limits>
#include <cstdlib>
#include <chrono>
#include <iomanip>
#include <thread>
#include <memory>
#include <sstream>

// Constructor implementation
Game::Game() : helicopter("AH-64 Apache"), gameState(GameState::MAIN_MENU), 
               gameRunning(true), realTimeMode(true), simSpeed(SimulationSpeed::REAL_TIME),
               deltaTime(0.0), gameTime(0.0), missionTime(0.0), timeAcceleration(1.0),
               pausedState(false), showDebugMode(false), showAdvancedInfo(false) {
    
    initializeHelicopter();
    setupDefaultWeapons();
    lastUpdateTime = std::chrono::steady_clock::now();
}

void Game::initializeHelicopter() {
    // Initialize helicopter with realistic systems
    // Already done in Helicopter constructor
}

void Game::setupDefaultWeapons() {
    // Enhanced weapons with realistic specifications
    WeaponSpecs aimSpecs = {15.0, 3.0, 5.0, 0.9, false, 50.0, 10.0};
    WeaponSpecs rocketSpecs = {8.0, 0.0, 3.0, 0.8, true, 30.0, 15.0};
    WeaponSpecs gunSpecs = {2.0, 0.0, 1.0, 0.7, true, 15.0, 2.0};
    WeaponSpecs atmSpecs = {12.0, 2.0, 8.0, 0.85, false, 60.0, 8.0};
    
    helicopter.addWeapon(Weapon("AIM-114 Hellfire", WeaponType::AIR_TO_GROUND_MISSILE, 80, 120, 8, atmSpecs));
    helicopter.addWeapon(Weapon("Hydra 70 Rockets", WeaponType::ROCKET_POD, 25, 45, 38, rocketSpecs));
    helicopter.addWeapon(Weapon("M230 Chain Gun", WeaponType::CANNON, 12, 28, 1200, gunSpecs));
    helicopter.addWeapon(Weapon("AIM-9X Sidewinder", WeaponType::AIR_TO_AIR_MISSILE, 60, 100, 4, aimSpecs));
    
    generateEnemies(MissionType::SEARCH_AND_DESTROY, 5);
}

void Game::generateEnemies(MissionType type, int difficulty) {
    enemies.clear();
    
    // Create realistic enemy formations based on mission type
    switch (type) {
        case MissionType::SEARCH_AND_DESTROY:
            enemies = createEnemyFormation(EnemyType::LIGHT_TANK, 2, EnemyPosition(5.0, 3.0, 0.0));
            enemies.push_back(Enemy(EnemyType::SAM_SITE, EnemyPosition(8.0, -2.0, 0.0)));
            enemies.push_back(Enemy(EnemyType::SCOUT_DRONE, EnemyPosition(3.0, 5.0, 50.0)));
            break;
        default:
            // Default enemy setup
            enemies.push_back(Enemy("Scout Drone", 30, 5, 15));
            enemies.push_back(Enemy("Attack Drone", 50, 10, 20));
            enemies.push_back(Enemy("Light Tank", 75, 15, 25));
            break;
    }
}

std::vector<Enemy> Game::createEnemyFormation(EnemyType type, int count, const EnemyPosition& centerPos) {
    std::vector<Enemy> formation;
    for (int i = 0; i < count; ++i) {
        EnemyPosition pos = centerPos;
        pos.x += (i % 2) * 1.0 - 0.5; // Spread formation
        pos.y += (i / 2) * 1.0;
        formation.emplace_back(type, pos);
    }
    return formation;
}

void Game::start() {
    gameRunning = true;
    
    while (gameRunning) {
        update();
        render();
        
        // Simple frame rate limiting
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void Game::update() {
    auto currentTime = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - lastUpdateTime);
    deltaTime = duration.count() / 1000000.0; // Convert to seconds
    lastUpdateTime = currentTime;
    
    if (!pausedState) {
        updateGameLogic(deltaTime * timeAcceleration);
    }
    
    processInput();
}

void Game::updateGameLogic(double dt) {
    gameTime += dt;
    
    updateHelicopter(dt);
    updateEnemies(dt);
    updateEnvironment(dt);
    
    if (currentMission) {
        updateMission(dt);
        missionTime += dt;
    }
    
    updateCombat(dt);
}

void Game::updateHelicopter(double dt) {
    helicopter.updatePosition(dt);
    
    // Update weapon systems
    for (int i = 0; i < helicopter.getWeaponCount(); ++i) {
        // Weapon updates would be handled in weapon system
    }
}

void Game::updateEnemies(double dt) {
    for (auto& enemy : enemies) {
        enemy.updatePosition(dt);
    }
}

void Game::updateEnvironment(double dt) {
    environment.updateWeather(dt);
    environment.updateTimeOfDay(dt);
}

void Game::updateMission(double dt) {
    currentMission->update(dt);
}

void Game::updateCombat(double dt) {
    // Combat logic updates
}

void Game::render() {
    // Simple text-based rendering
    if (gameState == GameState::IN_FLIGHT) {
        showFlightInterface();
    }
}

void Game::processInput() {
    // Handle continuous input processing
}

void Game::showMainMenu() {
    std::cout << "\n=== HELICOPTER COMBAT SIMULATOR ===" << std::endl;
    std::cout << "1. Start Mission" << std::endl;
    std::cout << "2. Free Flight" << std::endl;
    std::cout << "3. Training" << std::endl;
    std::cout << "4. Aircraft Status" << std::endl;
    std::cout << "5. Environmental Status" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "Enter your choice: ";
}

void Game::showMissionMenu() {
    std::cout << "\n=== MISSION SELECTION ===" << std::endl;
    std::cout << "1. Search & Destroy" << std::endl;
    std::cout << "2. Reconnaissance" << std::endl;
    std::cout << "3. Escort Mission" << std::endl;
    std::cout << "4. Rescue Operation" << std::endl;
    std::cout << "5. Close Air Support" << std::endl;
    std::cout << "0. Back to Main Menu" << std::endl;
    std::cout << "Enter your choice: ";
    
    int choice;
    std::cin >> choice;
    
    if (choice > 0 && choice <= 5) {
        startMission(static_cast<MissionType>(choice - 1));
    }
}

void Game::startMission(MissionType type) {
    // Create mission based on type
    std::string missionName;
    std::string briefing;
    
    switch (type) {
        case MissionType::SEARCH_AND_DESTROY:
            missionName = "Operation Desert Storm";
            briefing = "Locate and destroy enemy armor and air defense units in the target area.";
            break;
        case MissionType::RECONNAISSANCE:
            missionName = "Operation Eagle Eye";
            briefing = "Conduct reconnaissance of enemy positions while maintaining stealth.";
            break;
        default:
            missionName = "Training Mission";
            briefing = "Basic training scenario to familiarize with systems.";
            break;
    }
    
    currentMission = std::make_unique<Mission>(type, missionName, briefing);
    
    // Set mission parameters
    MissionParameters params;
    params.area = "Sector Alpha-7";
    params.terrain = environment.getCurrentTerrain();
    params.weather = environment.getCurrentWeather();
    params.timeLimit = 45.0; // 45 minutes
    params.stealthRequired = (type == MissionType::RECONNAISSANCE);
    
    currentMission->setParameters(params);
    currentMission->start();
    
    gameState = GameState::MISSION_BRIEFING;
    enterFlightMode();
}

void Game::enterFlightMode() {
    gameState = GameState::IN_FLIGHT;
    std::cout << "\n🚁 Entering flight mode..." << std::endl;
    
    int choice;
    do {
        showInFlightMenu();
        std::cin >> choice;
        handleInFlightInput(choice);
    } while (choice != 0 && gameState == GameState::IN_FLIGHT);
}

void Game::showInFlightMenu() {
    std::cout << "\n=== IN-FLIGHT OPERATIONS ===" << std::endl;
    std::cout << "1. Engage Enemy" << std::endl;
    std::cout << "2. Radar Sweep" << std::endl;
    std::cout << "3. Navigation" << std::endl;
    std::cout << "4. Systems Check" << std::endl;
    std::cout << "5. Mission Status" << std::endl;
    std::cout << "6. Environmental Info" << std::endl;
    std::cout << "7. Emergency Procedures" << std::endl;
    std::cout << "0. Return to Base" << std::endl;
    std::cout << "Enter your choice: ";
}

void Game::handleInFlightInput(int choice) {
    switch (choice) {
        case 1: engageEnemy(); break;
        case 2: performRadarScan(); break;
        case 3: showNavigationMap(); break;
        case 4: helicopter.performSystemCheck(); break;
        case 5: showMissionStatus(); break;
        case 6: showEnvironmentalStatus(); break;
        case 7: handleEmergency(); break;
        case 0: gameState = GameState::MAIN_MENU; break;
        default: std::cout << "Invalid choice." << std::endl; break;
    }
}

void Game::engageEnemy() {
    if (enemies.empty()) {
        std::cout << "No enemies detected in the area." << std::endl;
        return;
    }
    
    int targetIndex = selectEnemyTarget();
    if (targetIndex >= 0) {
        int weaponIndex = selectWeaponIndex();
        if (weaponIndex >= 0) {
            performAttack(targetIndex, weaponIndex);
        }
    }
}

int Game::selectEnemyTarget() {
    std::cout << "\n=== TARGET SELECTION ===" << std::endl;
    for (size_t i = 0; i < enemies.size(); ++i) {
        std::cout << (i+1) << ". " << enemies[i].getType() 
                  << " (Health: " << enemies[i].getHealth() << ")" << std::endl;
    }
    std::cout << "0. Cancel" << std::endl;
    
    int choice;
    std::cin >> choice;
    
    if (choice > 0 && choice <= static_cast<int>(enemies.size())) {
        return choice - 1;
    }
    return -1;
}

int Game::selectWeaponIndex() {
    std::cout << "\n=== WEAPON SELECTION ===" << std::endl;
    helicopter.listWeapons();
    std::cout << "0. Cancel" << std::endl;
    
    int choice;
    std::cin >> choice;
    
    if (choice > 0 && choice <= helicopter.getWeaponCount()) {
        return choice - 1;
    }
    return -1;
}

void Game::performAttack(int enemyIndex, int weaponIndex) {
    if (enemyIndex >= 0 && enemyIndex < static_cast<int>(enemies.size()) &&
        weaponIndex >= 0 && weaponIndex < helicopter.getWeaponCount()) {
        
        // Calculate distance to target
        double distance = 3.0; // Simplified distance calculation
        
        if (helicopter.attackWithWeapon(enemies[enemyIndex], weaponIndex, distance)) {
            if (enemies[enemyIndex].getHealth() <= 0) {
                std::cout << enemies[enemyIndex].getType() << " destroyed!" << std::endl;
                enemies.erase(enemies.begin() + enemyIndex);
                
                if (currentMission) {
                    // Check if this completes mission objectives
                    if (enemies.empty()) {
                        currentMission->completeObjective(0); // Assuming first objective is destroy enemies
                    }
                }
            } else {
                // Enemy counterattack
                processEnemyTurn();
            }
        }
    }
}

void Game::processEnemyTurn() {
    for (auto& enemy : enemies) {
        if (enemy.getHealth() > 0) {
            // Simple enemy AI - attack if in range
            int damage = enemy.attackDamage();
            std::cout << enemy.getType() << " counterattacks for " << damage << " damage!" << std::endl;
            helicopter.takeDamage(damage);
            
            if (!helicopter.isAlive()) {
                std::cout << "\n💥 HELICOPTER DESTROYED! MISSION FAILED!" << std::endl;
                if (currentMission) {
                    currentMission->fail("Aircraft destroyed");
                }
                gameState = GameState::MISSION_FAILED;
                return;
            }
        }
    }
}

void Game::performRadarScan() {
    helicopter.performRadarSweep(enemies, environment.getCurrentWeather());
}

void Game::showNavigationMap() {
    std::cout << "\n=== NAVIGATION ===" << std::endl;
    std::cout << "Current Position: (" << std::fixed << std::setprecision(1)
              << helicopter.getPosition().x << ", " << helicopter.getPosition().y 
              << ") at " << helicopter.getPosition().altitude << "m" << std::endl;
    std::cout << "Fuel: " << std::fixed << std::setprecision(0) 
              << helicopter.getFlightParams().fuel << " liters" << std::endl;
    std::cout << "Speed: " << helicopter.getFlightParams().speed << " km/h" << std::endl;
}

void Game::handleEmergency() {
    std::cout << "\n=== EMERGENCY PROCEDURES ===" << std::endl;
    std::cout << "1. Emergency Landing" << std::endl;
    std::cout << "2. Systems Restart" << std::endl;
    std::cout << "3. Request Support" << std::endl;
    std::cout << "4. Abort Mission" << std::endl;
    std::cout << "0. Cancel" << std::endl;
    
    int choice;
    std::cin >> choice;
    
    switch (choice) {
        case 1:
            helicopter.emergency_landing();
            break;
        case 2:
            std::cout << "Attempting systems restart..." << std::endl;
            break;
        case 3:
            std::cout << "Support request transmitted." << std::endl;
            break;
        case 4:
            if (currentMission) {
                currentMission->abort();
            }
            gameState = GameState::MAIN_MENU;
            break;
    }
}

void Game::showMissionStatus() {
    if (currentMission) {
        currentMission->showMissionStatus();
    } else {
        std::cout << "No active mission." << std::endl;
    }
}

void Game::showEnvironmentalStatus() {
    environment.showEnvironmentalStatus();
}

void Game::showSystemStatus() {
    helicopter.showDetailedStatus();
}

void Game::showSimulationControls() {
    std::cout << "\n=== SIMULATION CONTROLS ===" << std::endl;
    std::cout << "1. Pause/Resume" << std::endl;
    std::cout << "2. Adjust Time Speed" << std::endl;
    std::cout << "3. Toggle Real-time Mode" << std::endl;
    std::cout << "4. Weather Control" << std::endl;
    std::cout << "5. Debug Information" << std::endl;
    std::cout << "0. Back" << std::endl;
    
    int choice;
    std::cin >> choice;
    
    switch (choice) {
        case 1:
            pausedState = !pausedState;
            std::cout << "Simulation " << (pausedState ? "paused" : "resumed") << std::endl;
            break;
        case 2:
            std::cout << "Enter time acceleration (1.0 = real-time): ";
            std::cin >> timeAcceleration;
            break;
        case 3:
            toggleRealTime();
            std::cout << "Real-time mode " << (realTimeMode ? "enabled" : "disabled") << std::endl;
            break;
        case 4:
            std::cout << "Weather control not yet implemented." << std::endl;
            break;
        case 5:
            showDebugInfo();
            break;
    }
}

void Game::showDebugInfo() {
    std::cout << "\n=== DEBUG INFORMATION ===" << std::endl;
    std::cout << "Game Time: " << std::fixed << std::setprecision(1) << gameTime << "s" << std::endl;
    std::cout << "Delta Time: " << deltaTime << "s" << std::endl;
    std::cout << "Time Acceleration: " << timeAcceleration << "x" << std::endl;
    std::cout << "Enemies: " << enemies.size() << std::endl;
    std::cout << "Helicopter Alive: " << (helicopter.isAlive() ? "Yes" : "No") << std::endl;
}

// Legacy compatibility methods
void Game::showStatus() {
    helicopter.showDetailedStatus();
    showEnvironmentalStatus();
    if (currentMission) {
        showMissionStatus();
    }
}

void Game::selectTarget() {
    selectEnemyTarget();
}

void Game::selectWeapon() {
    selectWeaponIndex();
}

void Game::showFlightInterface() {
    std::cout << "\n=== FLIGHT INTERFACE ===" << std::endl;
    std::cout << "Time: " << formatTime(gameTime) << std::endl;
    std::cout << "Position: (" << std::fixed << std::setprecision(1)
              << helicopter.getPosition().x << ", " << helicopter.getPosition().y 
              << ") Alt: " << helicopter.getPosition().altitude << "m" << std::endl;
    std::cout << "Fuel: " << std::fixed << std::setprecision(0) 
              << helicopter.getFlightParams().fuel << "/" 
              << helicopter.getFlightParams().maxFuel << "L" << std::endl;
    std::cout << "Enemies: " << enemies.size() << " contacts" << std::endl;
    
    if (currentMission) {
        std::cout << "Mission: " << currentMission->getName() << std::endl;
        std::cout << "Progress: " << std::fixed << std::setprecision(0) 
                  << currentMission->getProgress() * 100 << "%" << std::endl;
    }
}

void Game::showMissionBriefing() {
    if (currentMission) {
        currentMission->showDetailedBriefing();
    } else {
        std::cout << "No active mission to brief." << std::endl;
    }
}

std::string Game::formatTime(double seconds) const {
    int minutes = static_cast<int>(seconds) / 60;
    int secs = static_cast<int>(seconds) % 60;
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << minutes << ":" 
       << std::setfill('0') << std::setw(2) << secs;
    return ss.str();
}

std::string Game::formatDistance(double km) const {
    if (km < 1.0) {
        return std::to_string(static_cast<int>(km * 1000)) + "m";
    } else {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(1) << km << "km";
        return ss.str();
    }
}

