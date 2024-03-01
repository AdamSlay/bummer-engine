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

    for (Entity &entity : entityManager.getEntities())
    {
        if (entity.hasComponent<Transform>() && entity.hasComponent<Collider>() && entity.hasComponent<Sprite>())
        {
            Transform &transform = entity.getComponent<Transform>();
            Collider &col = entity.getComponent<Collider>();
            Sprite &spr = entity.getComponent<Sprite>();

            int scaledW = static_cast<int>(spr.srcRect.w * transform.scale);
            int scaledH = static_cast<int>(spr.srcRect.h * transform.scale);
            SDL_Rect destRect = {transform.x, transform.y, scaledW, scaledH};
            if (entity.hasComponent<Velocity>()) {
                SDL_RendererFlip flip = (entity.getComponent<Velocity>().direction == -1) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
                SDL_RenderCopyEx(renderer, spr.texture, &spr.srcRect, &destRect, 0.0, nullptr, flip);
            }
            else {
                SDL_RenderCopy(renderer, spr.texture, &spr.srcRect, &destRect);
            }
        }
    }
}
