#pragma once
#include "Game.h"

class HelicopterCombat {
public:
    void run();
    void showMainMenu();
    void handleUserInput(int choice);

private:
    Game game;
};
