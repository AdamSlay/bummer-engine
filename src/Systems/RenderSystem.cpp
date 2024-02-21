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
        if (entity.hasComponent<Position>() && entity.hasComponent<Collider>() && entity.hasComponent<Sprite>()) {
            Position& pos = entity.getComponent<Position>();
            Collider& col = entity.getComponent<Collider>();
            Sprite& spr = entity.getComponent<Sprite>();
            SDL_Rect rect = {pos.x, pos.y, col.width, col.height};
            SDL_RenderCopy(renderer, spr.texture, nullptr, &rect);
        }
    }
}
