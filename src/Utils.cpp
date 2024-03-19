#include "Utils.h"


#include <iostream>

#include "ECS/Components.h"

SDL_Rect Utils::getColliderRect(const Entity& entity) {
    /**
     * Get the position of the collider
     *
     * @param entity: The entity
     * @return: A pair of integers representing the x and y position of the collider
     */
    const Transform& transform = entity.getComponent<Transform>();
    const Collider& collider = entity.getComponent<Collider>();
    int x = transform.x + (collider.offsetX * transform.scale);
    int y = transform.y + (collider.offsetY * transform.scale);
    int w = collider.width * transform.scale;
    int h = collider.height * transform.scale;
    return SDL_Rect {x, y, w, h};
}

void Utils::setTransformPos(Entity& entity, int x, int y) {
    Transform& transform = entity.getComponent<Transform>();
    const Collider& collider = entity.getComponent<Collider>();
    transform.x = x - (collider.offsetX * transform.scale);
    transform.y = y - (collider.offsetY * transform.scale);
}

void Utils::setTransformX(Entity& entity, int x) {
    Transform& transform = entity.getComponent<Transform>();
    const Collider& collider = entity.getComponent<Collider>();
    transform.x = x - (collider.offsetX * transform.scale);
}

void Utils::setTransformY(Entity& entity, int y) {
    Transform& transform = entity.getComponent<Transform>();
    const Collider& collider = entity.getComponent<Collider>();
    transform.y = y - (collider.offsetY * transform.scale);
}

void Utils::render_collider(Entity &entity, SDL_Renderer *renderer) {
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

void Utils::render_all_colliders(EntityManager& entityManager, SDL_Renderer* renderer) {
    /**
     * Render all colliders for all entities
     *
     * @param renderer: The SDL renderer
     * @param entityManager: The entity manager
     */
    for (Entity& entity : entityManager.getEntities()) {
        if (entity.hasComponent<Collider>()) {
            Utils::render_collider(entity, renderer);
        }
    }
}

SDL_Rect Utils::getHitboxRect(Hitbox& hitbox, Transform& transform) {
    SDL_Rect hitboxRect;
    hitboxRect.x = transform.x + (hitbox.offsetX * transform.scale);
    hitboxRect.y = transform.y + (hitbox.offsetY * transform.scale);
    hitboxRect.w = hitbox.width;
    hitboxRect.h = hitbox.height;

    std::cout << "Hitbox: " << hitboxRect.x << " " << hitboxRect.y << " " << hitboxRect.w << " " << hitboxRect.h << std::endl;

    return hitboxRect;
}