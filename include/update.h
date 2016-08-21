#ifndef __UPDATE_H_INCLUDED__
#define __UPDATE_H_INCLUDED__

#include <SFML/Graphics.hpp>
#include "player.h"
#include "level.h"

namespace Update {
    extern int state;
    extern int weaponOffset;
    extern double timer;
    extern double openDoorTimer;
    extern bool openingDoor;
    extern bool waitingDoor;
    extern bool closeDoor;

    extern int doorRow;
    extern int doorCol;

    void checkWindowState(sf::RenderWindow&);
    void checkTitleToGameState();
    void checkMovement(Player&, Level&);
    void checkWeapon(Player&, Level&);
    void checkDoor(Player&, Level&);
    void checkEnemies(Player&, Level&);
    void moveFireball(Player&, Level&);
    void checkPlayer(Player&);
}

#endif
