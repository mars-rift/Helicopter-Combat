#include "Helicopter.h"
#include "Weapon.h"
#include "Enemy.h"
#include <iostream>

int main() {
    Helicopter heli("Apache");
    Weapon gun("Machine Gun", 20, 30);
    Weapon missile("Missile", 40, 60);

    heli.addWeapon(gun);
    heli.addWeapon(missile);

    Enemy enemy("Tank", 100);

    heli.showStatus();
    heli.attack(enemy);

    std::cout << "Enemy health after attack: " << enemy.getHealth() << std::endl;

    heli.printDamageTable();

    return 0;
}
