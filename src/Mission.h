#pragma once
#include <string>
#include <vector>
#include <map>
#include "Enemy.h"
#include "Helicopter.h"

enum class MissionType {
    SEARCH_AND_DESTROY,
    RECONNAISSANCE,
    ESCORT,
    RESCUE,
    SUPPLY_DROP,
    GROUND_SUPPORT,
    PATROL
};

enum class MissionStatus {
    NOT_STARTED,
    IN_PROGRESS,
    COMPLETED,
    FAILED,
    ABORTED
};

struct Objective {
    std::string description;
    bool completed;
    bool critical;          // Mission fails if critical objective fails
    int priority;           // 1-10, higher = more important
    
    Objective(const std::string& desc, bool crit = false, int prio = 5) 
        : description(desc), completed(false), critical(crit), priority(prio) {}
};

struct MissionParameters {
    std::string area;               // e.g., "Desert Valley", "Urban Zone Alpha"
    TerrainType terrain;
    WeatherCondition weather;
    double timeLimit;               // minutes, -1 for no limit
    double allowedCasualties;       // percentage
    bool stealthRequired;
    std::vector<EnemyPosition> threatAreas;
    std::vector<EnemyPosition> noFlyZones;
};

class Mission {
public:
    Mission(MissionType type, const std::string& name, const std::string& briefing);
    
    // Mission lifecycle
    void start();
    void update(double deltaTime);
    void complete();
    void fail(const std::string& reason = "");
    void abort();
    
    // Objectives management
    void addObjective(const Objective& objective);
    void completeObjective(size_t index);
    bool areAllObjectivesComplete() const;
    bool hasCriticalObjectiveFailed() const;
    
    // Mission information
    MissionType getType() const { return missionType; }
    MissionStatus getStatus() const { return status; }
    std::string getName() const { return missionName; }
    std::string getBriefing() const { return briefing; }
    const MissionParameters& getParameters() const { return parameters; }
    
    // Progress tracking
    double getProgress() const;
    double getTimeRemaining() const;
    void showMissionStatus() const;
    void showDetailedBriefing() const;
    
    // Enemy and threat management
    void addThreat(const Enemy& enemy);
    void removeThreat(size_t index);
    const std::vector<Enemy>& getThreats() const { return threats; }
    
    // Scoring and evaluation
    int calculateScore() const;
    std::string getMissionRating() const;
    
    // Mission parameters
    void setParameters(const MissionParameters& params) { parameters = params; }
    bool isInNoFlyZone(const Position& pos) const;
    bool isInThreatArea(const Position& pos) const;

private:
    // Core mission data
    MissionType missionType;
    MissionStatus status;
    std::string missionName;
    std::string briefing;
    MissionParameters parameters;
    
    // Objectives and progress
    std::vector<Objective> objectives;
    double startTime;
    double elapsedTime;
    
    // Threats and enemies
    std::vector<Enemy> threats;
    
    // Performance metrics
    int enemiesDestroyed;
    int damageTaken;
    bool stealthMaintained;
    double accuracyRating;
    
    // Private helper methods
    void generateObjectives();
    void updateProgress(double deltaTime);
    bool checkFailureConditions() const;
    double calculateDistance(const Position& pos1, const EnemyPosition& pos2) const;
};
