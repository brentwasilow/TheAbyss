#ifndef __LEVEL_H_INCLUDED__
#define __LEVEL_H_INCLUDED__

#include <vector>
#include "player.h"

class Level {
    public:
        std::vector< std::vector<int> > map;

        Level(const char*, Player&);
};

#endif
