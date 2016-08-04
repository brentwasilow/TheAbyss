#ifndef __UPDATE_H_INCLUDED__
#define __UPDATE_H_INCLUDED__

#include <SFML/Graphics.hpp>
#include "player.h"
#include "level.h"

namespace Update {
    extern int state;
    extern int weaponOffset;
    extern double timer;

    void checkWindowState(sf::RenderWindow&);
    void checkTitleToGameState();
    void checkMovement(Player&, Level&);
    void checkWeapon(Player&);
}

#endif
