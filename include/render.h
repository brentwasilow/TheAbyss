#ifndef __RENDER_H_INCLUDED__
#define __RENDER_H_INCLUDED__

#include <SFML/Graphics.hpp>
#include "player.h"

namespace Render {
    void initialize(sf::RenderWindow&);
    void drawTitleScreen(sf::RenderWindow&);
    void drawBackground(sf::RenderWindow&);
    void drawMap(sf::RenderWindow&, Player&);
};

#endif
