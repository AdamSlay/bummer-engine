#ifndef BUMMERENGINE_COMPONENTS_H
#define BUMMERENGINE_COMPONENTS_H

#include <SDL2/SDL.h>

struct Player {
    int playerNumber;
};

struct Position {
    int x, y;
};

struct Velocity {
    int dx, dy;
};

struct Collider {
    int width, height;
};

struct Scale {
    float scale;
};

struct Sprite {
    SDL_Texture* texture;
    SDL_Rect srcRect;
};

#endif //BUMMERENGINE_COMPONENTS_H