#include "RenderSystem.h"
#include "../ECS/Components.h"

void RenderSystem::render(SDL_Renderer* renderer, EntityManager& entityManager) {
    /**
     * Renders all entities with a position and collider component
     *
     * @param renderer: The SDL renderer
     * @param entityManager: The entity manager
     */

    SDL_SetRenderDrawColor(renderer, 36, 188, 148, 255);  // bb_green

    for (Entity& entity : entityManager.getEntities()) {
        if (entity.hasComponent<Position>() && entity.hasComponent<Collider>()) {
            Position& pos = entity.getComponent<Position>();
            Collider& col = entity.getComponent<Collider>();
            SDL_Rect rect = {pos.x, pos.y, col.width, col.height};
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}
