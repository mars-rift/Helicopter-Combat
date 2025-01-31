#pragma once
#include "Helicopter.h"
#include "Enemy.h"

class Game {
public:
    Game();
    void start();
    void showMenu();
    void handleInput(int choice);

private:
    Helicopter helicopter;
    std::vector<Enemy> enemies;
};
