#ifndef BUMMERENGINE_RENDERSYSTEM_H
#define BUMMERENGINE_RENDERSYSTEM_H

#include <SDL2/SDL.h>

#include "../ECS/EntityManager.h"

class RenderSystem {
public:
    void render(SDL_Renderer* renderer, EntityManager& entityManager);
};


#endif //BUMMERENGINE_RENDERSYSTEM_H
