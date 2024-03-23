#ifndef BUMMERENGINE_RENDERSYSTEM_H
#define BUMMERENGINE_RENDERSYSTEM_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../ECS/EntityManager.h"

class RenderSystem {
public:
    void render(SDL_Renderer* renderer, EntityManager& entityManager, TTF_Font* font);
};


#endif //BUMMERENGINE_RENDERSYSTEM_H
