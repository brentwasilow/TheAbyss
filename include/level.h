#ifndef __LEVEL_H_INCLUDED__
#define __LEVEL_H_INCLUDED__

#include <vector>
#include "player.h"
#include "enemy.h"

class Level {
    public:
        std::vector< std::vector<int> > map;
        std::vector<Enemy> enemies;
        std::vector<double> zBuffer;

        Level(const char*, Player&);
};

#endif
