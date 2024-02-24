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
            Scale& scale = entity.getComponent<Scale>();

            int scaledW = static_cast<int>(col.width * scale.scale);
            int scaledH = static_cast<int>(col.height * scale.scale);
            SDL_Rect destRect = {pos.x, pos.y, scaledW, scaledH};
            SDL_RenderCopy(renderer, spr.texture, &spr.srcRect, &destRect);
        }
    }
}
