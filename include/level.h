#ifndef __LEVEL_H_INCLUDED__
#define __LEVEL_H_INCLUDED__

#include <vector>
#include "player.h"
#include "sprite.h"

class Level {
    public:
        std::vector< std::vector<int> > map;
        std::vector<Sprite*> sprites;
        std::vector<double> zBuffer;

        Level(const char*, Player&);
};

#endif
