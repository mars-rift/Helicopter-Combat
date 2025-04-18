#pragma once
#include "Helicopter.h"
#include "Enemy.h"
#include <vector>

class Game {
public:
    Game();  // This is the constructor declaration
    void start();
    void showMenu();
    void handleInput(int choice);
    void showStatus();
    int selectEnemy();
    int selectWeapon();

private:
    Helicopter helicopter;
    std::vector<Enemy> enemies;
};
