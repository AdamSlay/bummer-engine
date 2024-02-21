#include "RenderSystem.h"
#include "../ECS/Components.h"

void RenderSystem::render(SDL_Renderer* renderer, EntityManager& entityManager) {
    for (Entity& entity : entityManager.getEntities()) {
        if (entity.hasComponent<Position>() && entity.hasComponent<Collider>()) {
            Position& pos = entity.getComponent<Position>();
            Collider& col = entity.getComponent<Collider>();
            SDL_Rect rect = {pos.x, pos.y, col.width, col.height};
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}
