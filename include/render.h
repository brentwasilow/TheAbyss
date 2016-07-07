#ifndef __RENDER_H_INCLUDED__
#define __RENDER_H_INCLUDED__

#include <SFML/Graphics.hpp>
#include "player.h"
#include "level.h"

namespace Render {
    void initialize(sf::RenderWindow&);
    void drawTitleScreen(sf::RenderWindow&);
    void drawBackground(sf::RenderWindow&);
    void drawMap(sf::RenderWindow&, Player&, Level&);
    void drawEnemies(sf::RenderWindow&, Player&, Level&);
};

#endif
