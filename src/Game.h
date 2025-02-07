#pragma once
#include "Helicopter.h"
#include "Enemy.h"
#include <vector>

class Game {
public:
    Game();
    void start();
    void showMenu();
    void handleInput(int choice);
    void showStatus();

private:
    Helicopter helicopter;
    std::vector<Enemy> enemies;
};
