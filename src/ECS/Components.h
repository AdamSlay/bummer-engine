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

struct Sprite {
    SDL_Texture* texture;
};

#endif //BUMMERENGINE_COMPONENTS_H