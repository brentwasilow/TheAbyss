#ifndef __RENDER_H_INCLUDED__
#define __RENDER_H_INCLUDED__

#include <SFML/Graphics.hpp>

namespace Render {
    double verticalIntersection(double);
    double horizontalIntersection(double);
    void init(sf::RenderWindow&);
    void drawTitleScreen(sf::RenderWindow&);
    void drawBackground(sf::RenderWindow&);
    void drawMap(sf::RenderWindow&);
};

#endif
