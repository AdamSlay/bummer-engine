#include "RenderSystem.h"
#include "../ECS/Components.h"
#include "../UI/Menu.h"
#include "../Config.h"
#include "../Utils.h"

void RenderSystem::render(SDL_Renderer* renderer, EntityManager& entityManager, TTF_Font* font) {
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

            // Visual Debugging
//            if (entity.hasComponent<State>()) {
//                auto& state = entity.getComponent<State>();
//                std::string stateStr = Utils::playerStateToString(state.state); // Assuming you have such a function
//                SDL_Color color = {255, 255, 255}; // White color for text
//                render_text(renderer, font, stateStr, color, transform.x, transform.y + 20); // Assuming you have such a function
//            }

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


void RenderSystem::render_hitboxes(EntityManager &entityManager, SDL_Renderer *renderer) {
    /**
     * Render all active hitboxes for all entities
     *
     * @param renderer: The SDL renderer
     * @param entityManager: The entity manager
     */

    for (Entity& entity : entityManager.getEntities()) {
        if (entity.hasComponent<AttackMap>()) {
            for (auto& [name, attackInfo] : entity.getComponent<AttackMap>().attacks) {
                if (attackInfo.isActive) {
                    Hitbox& hitbox = attackInfo.hitbox;
                    SDL_Rect hitboxRect = Utils::getHitboxRect(hitbox, entity);
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                    SDL_RenderDrawRect(renderer, &hitboxRect);
                }
            }
        }
    }
}

void RenderSystem::render_all_colliders(EntityManager& entityManager, SDL_Renderer* renderer) {
    /**
     * Render all colliders for all entities
     *
     * @param renderer: The SDL renderer
     * @param entityManager: The entity manager
     */

    for (Entity& entity : entityManager.getEntities()) {
        if (entity.hasComponent<Collider>()) {
            render_collider(entity, renderer);
        }
    }
}

void RenderSystem::render_collider(Entity &entity, SDL_Renderer *renderer) {
    /**
     * Render the collider for an entity
     *
     * @param entity: The entity
     * @param renderer: The SDL renderer
     */

    Transform &transform = entity.getComponent<Transform>();
    Collider &col = entity.getComponent<Collider>();
    int x = transform.x + (col.offsetX * transform.scale);
    int y = transform.y + (col.offsetY * transform.scale);
    int w = col.width * transform.scale;
    int h = col.height * transform.scale;
    SDL_Rect collider = {x, y, w, h};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &collider);
}

