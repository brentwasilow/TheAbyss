#ifndef __LEVEL_H_INCLUDED__
#define __LEVEL_H_INCLUDED__

#include <vector>

namespace Level {
    extern std::vector<std::vector<int> > map;

    void loadMap(const char*);
};

#endif
