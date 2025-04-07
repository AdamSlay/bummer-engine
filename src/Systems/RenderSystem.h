#pragma once
#ifndef BUMMERENGINE_RENDERSYSTEM_H
#define BUMMERENGINE_RENDERSYSTEM_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../ECS/EntityManager.h"
#include "Camera.h"

class RenderSystem {
public:
    RenderSystem();
    void render(SDL_Renderer* renderer, EntityManager& entityManager, TTF_Font* font);
    void render_hitboxes(EntityManager& entityManager, SDL_Renderer* renderer);
    void render_all_colliders(EntityManager& entityManager, SDL_Renderer* renderer);
    void render_collider(Entity& entity, SDL_Renderer* renderer);

private:
    Camera camera;
};

#endif //BUMMERENGINE_RENDERSYSTEM_H