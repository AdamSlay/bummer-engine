#ifndef BUMMERENGINE_COMPONENTS_H
#define BUMMERENGINE_COMPONENTS_H

#include <SDL2/SDL.h>

struct Position {
    int x, y;
};

struct Collider {
    int width, height;
};

struct Scale {
    float scale;
};

#endif //BUMMERENGINE_COMPONENTS_H