#include "CollisionSystem.h"
#include "../ECS/EventManager.h"
#include "../Utils.h"

CollisionSystem::CollisionSystem() {
    /**
     * Constructor for the CollisionSystem
     *
     * The collisionBuffer defined here is the number of pixels that entities' colliders should be separated by
     * when a collision is detected. This is to prevent entities from continually colliding with each other.
     */
    collisionBuffer = 0;
}

void CollisionSystem::update(EntityManager& entityManager) {
    /**
     * Iterate through all movable + collidable entities and check for collisions with other collidable entities
     *
     * @param entityManager: The EntityManager
     */
    auto& movableCollidableEntities = entityManager.getMovableCollidableEntities();
    auto& collidableEntities = entityManager.getCollidableEntities();

    for (auto& primaryEntity : movableCollidableEntities) {
        bool collision = false;
        for (auto& otherEntity : collidableEntities) {

            if (primaryEntity.id != otherEntity.id && checkCollision(primaryEntity, otherEntity)){
                collision = true;
                handleCollision(primaryEntity, otherEntity);
            }
        }
        if (!collision) {
            // publish airborne event if the entity is not colliding with anything
            // groundCollision is published if the entity is colliding with the ground
            EventManager::getInstance().publish("airborne", {&primaryEntity});
        }
    }
}

bool CollisionSystem::checkCollision(Entity &primaryEntity, Entity &otherEntity) {
    /**
     * Check if the primaryEntity collider is intersecting with the otherEntity collider on both the X and Y axis
     *
     * @param primaryEntity: The primary Entity
     * @param otherEntity: The other Entity
     */
    return checkCollisionX(primaryEntity, otherEntity) && checkCollisionY(primaryEntity, otherEntity);
}

void CollisionSystem::handleCollision(Entity& primaryEntity, Entity& otherEntity) {
    /**
     * Determine the type of collision and handle it.
     *
     * The intersection rectangle is used to determine the type of collision
     * where the intersection rectangle is the area where the two colliders overlap.
     *
     * If the height of the intersection rectangle is greater than the width, the collision is on the X axis,
     * and the primaryEntity will be stopped and repositioned to the left or right of the otherEntity.
     *
     * If the width of the intersection rectangle is greater than or equal to the height, the collision is on the Y axis,
     * and the primaryEntity will be stopped and repositioned above or below the otherEntity.
     *
     * @param primaryEntity: The primary Entity
     * @param otherEntity: The other Entity
     */
    SDL_Rect primaryCollider = primaryEntity.getColliderRect();
    SDL_Rect otherCollider = otherEntity.getColliderRect();

    SDL_Rect intersection_rect;
    SDL_IntersectRect(&primaryCollider, &otherCollider, &intersection_rect);

    if (intersection_rect.h > intersection_rect.w && intersection_rect.w < primaryCollider.w) {
        handleCollisionX(primaryEntity, otherEntity);
    }
    else {
        handleCollisionY(primaryEntity, otherEntity);
    }
}

bool CollisionSystem::checkCollisionX(Entity &primaryEntity, Entity &otherEntity) {
    /**
     * Check if the primaryEntity is colliding with another Entity on the X axis
     *
     * @param primaryEntity: The primary Entity
     * @param otherEntity: The other Entity
     */
    SDL_Rect primaryCollider = primaryEntity.getColliderRect();
    SDL_Rect otherCollider = otherEntity.getColliderRect();

    if (isTouchingXaxis(primaryCollider, otherCollider)) {
        return true;
    }

    return false;
}

bool CollisionSystem::isTouchingXaxis(const SDL_Rect &primaryCollider, const SDL_Rect &otherCollider) {
    /**
     * Check if the primaryEntity is colliding with the otherEntity on the X axis
     *
     * @param primaryCollider: The primaryEntity collider
     * @param otherCollider: The otherEntity collider
     */
    return !isLeftOf(primaryCollider, otherCollider) && !isRightOf(primaryCollider, otherCollider);
}

bool CollisionSystem::isLeftOf(const SDL_Rect &primaryCollider, const SDL_Rect &otherCollider) {
    /**
     * Check if the primaryEntity is to the left of and not touching otherEntity
     *
     * @param primaryCollider: The primaryEntity collider
     * @param otherCollider: The otherEntity collider
     */
    return primaryCollider.x + primaryCollider.w < otherCollider.x;
}

bool CollisionSystem::isRightOf(const SDL_Rect &primaryCollider, const SDL_Rect &otherCollider) {
    /**
     * Check if the primaryEntity is to the right of and not touching otherEntity
     *
     * @param primaryCollider: The primaryEntity collider
     * @param otherCollider: The otherEntity collider
     */
    return primaryCollider.x > otherCollider.x + otherCollider.w;
}

bool CollisionSystem::checkCollisionY(Entity &primaryEntity, Entity &otherEntity) {
    /**
     * Check if the primaryEntity is colliding with otherEntity on the Y axis
     *
     * @param primaryEntity: The primary Entity
     * @param otherEntity: The other Entity
     */
    SDL_Rect primaryCollider = primaryEntity.getColliderRect();
    SDL_Rect otherCollider = otherEntity.getColliderRect();

    if (isTouchingYaxis(primaryCollider, otherCollider)) {
        return true;
    }

    return false;
}

bool CollisionSystem::isTouchingYaxis(const SDL_Rect &primaryCollider, const SDL_Rect &otherCollider) {
    /**
     * Check if the primaryEntity is touching otherEntity on the Y axis
     *
     * @param primaryCollider: The primaryEntity collider
     * @param otherCollider: The otherEntity collider
     */
    return !isAbove(primaryCollider, otherCollider) && !isBelow(primaryCollider, otherCollider);
}

bool CollisionSystem::isAbove(const SDL_Rect &primaryCollider, const SDL_Rect &otherCollider) {
    /**
     * Check if the primaryEntity is above otherEntity
     *
     * @param primaryCollider: The primaryEntity collider
     * @param otherCollider: The otherEntity collider
     */
    return primaryCollider.y + primaryCollider.h < otherCollider.y;
}

bool CollisionSystem::isBelow(const SDL_Rect &primaryCollider, const SDL_Rect &otherCollider) {
    /**
     * Check if the primaryEntity is below the otherEntity
     *
     * @param primaryCollider: The primaryEntity collider
     * @param otherCollider: The otherEntity collider
     */
    return primaryCollider.y > otherCollider.y + otherCollider.h;
}

void CollisionSystem::handleCollisionX(Entity& primaryEntity, Entity& otherEntity) {
    /**
     * Handle primaryEntity collision with otherEntity on the X axis
     *
     * @param entity: The primaryEntity primaryEntity
     * @param other: The otherEntity primaryEntity
     */
    SDL_Rect primaryCollider = primaryEntity.getColliderRect();
    SDL_Rect otherCollider = otherEntity.getColliderRect();

    if (primaryCollider.x < otherCollider.x) {  // primaryEntity is to the left of other otherEntity
        stopAndRepositionToLeft(primaryEntity, primaryCollider, otherCollider);
    }
    else if (primaryCollider.x > otherCollider.y) {  // primaryEntity is to the right of other otherEntity
        stopAndRepositionToRight(primaryEntity, otherCollider);
    }
}

void CollisionSystem::stopAndRepositionToLeft(Entity& primaryEntity, const SDL_Rect& primaryCollider, const SDL_Rect& otherCollider) {
    /**
     * Stop the primaryEntity and reposition it to the left of the other Entity
     *
     * @param entity: The primaryEntity
     * @param entityCollider: The player collider
     * @param otherCollider: The other collider
     */
    auto& velocity = primaryEntity.getComponent<Velocity>();
    velocity.dx = 0;
    int newX = otherCollider.x - (primaryCollider.w + collisionBuffer);
    primaryEntity.setTransformX(newX);
}

void CollisionSystem::stopAndRepositionToRight(Entity& primaryEntity, const SDL_Rect& otherCollider) {
    /**
     * Stop the primaryEntity and reposition it to the right of the other Entity
     *
     * @param entity: The primaryEntity
     * @param playerCollider: The player collider
     * @param otherCollider: The other collider
     */
    auto& velocity = primaryEntity.getComponent<Velocity>();
    velocity.dx = 0;
    int newX = otherCollider.x + otherCollider.w + collisionBuffer;
    primaryEntity.setTransformX(newX);
}

void CollisionSystem::handleCollisionY(Entity& primaryEntity, Entity& otherEntity) {
    /**
     * Handle primaryEntity collision with otherEntity on the Y axis
     *
     * @param primaryEntity: The primary Entity
     * @param otherEntity: The other Entity
     */
    SDL_Rect primaryCollider = primaryEntity.getColliderRect();
    SDL_Rect otherCollider = otherEntity.getColliderRect();

    if (primaryCollider.y < otherCollider.y) {  // primaryEntity is above other otherEntity
        stopAndRepositionAbove(primaryEntity, primaryCollider, otherCollider);
    }
    else {  // primaryEntity is below other otherEntity
        stopAndRepositionBelow(primaryEntity, otherCollider);
    }
}

void CollisionSystem::stopAndRepositionAbove(Entity& primaryEntity, const SDL_Rect& primaryCollider, const SDL_Rect& otherCollider) {
    /**
     * Stop the primaryEntity and reposition it above the otherEntity
     *
     * @param primaryEntity: The primary Entity
     * @param primaryCollider: The primary Entity collider
     * @param otherCollider: The other Entity collider
     */
    EventManager::getInstance().publish("groundCollision", {&primaryEntity});

    auto& velocity = primaryEntity.getComponent<Velocity>();
    velocity.dy = 0;

    int newY = otherCollider.y - (primaryCollider.h);
    primaryEntity.setTransformY(newY);
}

void CollisionSystem::stopAndRepositionBelow(Entity& primaryEntity, const SDL_Rect& otherCollider) {
    /**
     * Stop the primaryEntity and reposition it below the otherEntity
     *
     * @param entity: The primary Entity
     * @param primaryCollider: The primary Entity collider
     * @param otherCollider: The other Entity collider
     */
    auto& velocity = primaryEntity.getComponent<Velocity>();
    velocity.dy = 0;
    int newY = otherCollider.y + otherCollider.h + collisionBuffer;
    primaryEntity.setTransformY(newY);
}