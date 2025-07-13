#pragma once
#include "Helicopter.h"
#include "Enemy.h"
#include "Mission.h"
#include "Environment.h"
#include <vector>
#include <memory>
#include <chrono>

enum class GameState {
    MAIN_MENU,
    MISSION_BRIEFING,
    IN_FLIGHT,
    MISSION_COMPLETE,
    MISSION_FAILED,
    PAUSED
};

enum class SimulationSpeed {
    REAL_TIME,
    FAST,
    VERY_FAST,
    PAUSED
};

class Game {
public:
    Game();
    
    // Main game loop
    void start();
    void update();
    void render();
    
    // Menu systems
    void showMainMenu();
    void showMissionMenu();
    void showSimulationMenu();
    void showInFlightMenu();
    void handleMainMenuInput(int choice);
    void handleMissionInput(int choice);
    void handleInFlightInput(int choice);
    
    // Mission management
    void startMission(MissionType type);
    void selectMission();
    void loadMission(const std::string& missionFile);
    void completeMission();
    void abortMission();
    void showMissionBriefing();
    void showMissionStatus();
    
    // Flight operations
    void enterFlightMode();
    void showFlightInterface();
    void performRadarScan();
    void showNavigationMap();
    void handleEmergency();
    
    // Combat system
    void engageEnemy();
    void selectTarget();
    void selectWeapon();
    int selectEnemyTarget();
    int selectWeaponIndex();
    void performAttack(int enemyIndex, int weaponIndex);
    void processEnemyTurn();
    void checkCombatResults();
    
    // Simulation controls
    void pauseSimulation();
    void resumeSimulation();
    void setSimulationSpeed(SimulationSpeed speed);
    void showSimulationControls();
    void toggleRealTime() { realTimeMode = !realTimeMode; }
    
    // Status and information
    void showStatus();
    void showDetailedStatus();
    void showSystemStatus();
    void showEnvironmentalStatus();
    void showTacticalDisplay();
    void showDebugInfo();
    
    // Game state management
    GameState getGameState() const { return gameState; }
    void setGameState(GameState state) { gameState = state; }
    bool isGameRunning() const { return gameRunning; }
    void exitGame() { gameRunning = false; }

private:
    // Core game objects
    Helicopter helicopter;
    std::vector<Enemy> enemies;
    std::unique_ptr<Mission> currentMission;
    Environment environment;
    
    // Game state
    GameState gameState;
    bool gameRunning;
    bool realTimeMode;
    SimulationSpeed simSpeed;
    
    // Timing
    std::chrono::steady_clock::time_point lastUpdateTime;
    double deltaTime;           // seconds
    double gameTime;           // total elapsed time
    double missionTime;        // time since mission start
    
    // Simulation parameters
    double timeAcceleration;   // 1.0 = real-time, higher = faster
    bool pausedState;
    
    // UI and display
    bool showDebugMode;
    bool showAdvancedInfo;
    
    // Input handling
    void processInput();
    void handleMenuNavigation();
    bool validateInput(int choice, int minChoice, int maxChoice);
    void clearInputBuffer();
    
    // Game mechanics
    void updateGameLogic(double deltaTime);
    void updateMission(double deltaTime);
    void updateHelicopter(double deltaTime);
    void updateEnemies(double deltaTime);
    void updateEnvironment(double deltaTime);
    void updateCombat(double deltaTime);
    
    // Mission generation
    void generateRandomMission();
    void generateEnemies(MissionType type, int difficulty);
    std::vector<Enemy> createEnemyFormation(EnemyType type, int count, 
                                          const EnemyPosition& centerPos);
    
    // Helper methods
    double calculateDeltaTime();
    void resetGame();
    void initializeHelicopter();
    void setupDefaultWeapons();
    bool checkGameOverConditions();
    void saveGameState();
    void loadGameState();
    
    // Display helpers
    void clearScreen();
    void showHeader();
    void showGameStats();
    void showControls();
    std::string formatTime(double seconds) const;
    std::string formatDistance(double km) const;
    
    // Movement functions for navigation
    void moveHelicopter(double deltaX, double deltaY);
    void moveToContact();
    void setCustomWaypoint();
    void changeAltitude();
    void adjustSpeed();
};
