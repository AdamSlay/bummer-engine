#include "CollisionSystem.h"
#include "../ECS/Components.h"
#include "../ECS/EventManager.h"
#include "../Utils.h"

#include <iostream>


void CollisionSystem::update(EntityManager &entityManager) {
    /**
     * Update the collision system
     *
     * @param entityManager: The EntityManager
     */
    auto& movableCollidableEntities = entityManager.getMovableCollidableEntities();
    auto& collidableEntities = entityManager.getCollidableEntities();

    for (auto& primaryEntity : movableCollidableEntities) {
        for (auto& otherEntity : collidableEntities) {
            if (primaryEntity.id != otherEntity.id && checkCollision(primaryEntity, otherEntity)) {
                handleCollision(primaryEntity, otherEntity);
            }
        }
    }
}

void CollisionSystem::handleCollision(Entity& primaryEntity, Entity& otherEntity) {
    /**
     * Handle primaryEntity collision with otherEntity
     *
     * @param primaryEntity: The primary Entity
     * @param otherEntity: The other Entity
     */
    SDL_Rect primaryCollider = primaryEntity.getColliderRect();
    SDL_Rect otherCollider = otherEntity.getColliderRect();

    SDL_Rect intersection_rect;
    SDL_IntersectRect(&primaryCollider, &otherCollider, &intersection_rect);

    if (intersection_rect.h > intersection_rect.w) {
        handleCollisionX(primaryEntity, otherEntity);
    }
    else {
        handleCollisionY(primaryEntity, otherEntity);
    }
}

bool CollisionSystem::checkCollision(Entity &primaryEntity, Entity &otherEntity) {
    /**
     * Check if the primaryEntity is colliding with another Entity on both the X and Y axis
     *
     * @param primaryEntity: The primary Entity
     * @param otherEntity: The other Entity
     */
    return checkCollisionX(primaryEntity, otherEntity) && checkCollisionY(primaryEntity, otherEntity);
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

    if (notTouchingXaxis(primaryCollider, otherCollider)) {
        return false;
    }

    return true;
}

bool CollisionSystem::notTouchingXaxis(const SDL_Rect &primaryCollider, const SDL_Rect &otherCollider) {
    /**
     * Check if the primaryEntity is touching otherEntity on the X axis
     *
     * @param primaryCollider: The primaryEntity collider
     * @param otherCollider: The otherEntity collider
     */
    return notTouchingAndLeftOf(primaryCollider, otherCollider) || notTouchingAndRightOf(primaryCollider, otherCollider);
}

bool CollisionSystem::notTouchingAndLeftOf(const SDL_Rect &primaryCollider, const SDL_Rect &otherCollider) {
    /**
     * Check if the primaryEntity is to the left of and not touching otherEntity
     *
     * @param primaryCollider: The primaryEntity collider
     * @param otherCollider: The otherEntity collider
     */
    return primaryCollider.x + primaryCollider.w < otherCollider.x;
}

bool CollisionSystem::notTouchingAndRightOf(const SDL_Rect &primaryCollider, const SDL_Rect &otherCollider) {
    /**
     * Check if the primaryEntity is to the right of not touching otherEntity
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

    if (notTouchingYaxis(primaryCollider, otherCollider)) {
        return false;
    }

    return true;
}

bool CollisionSystem::notTouchingYaxis(const SDL_Rect &primaryCollider, const SDL_Rect &otherCollider) {
    /**
     * Check if the primaryEntity is touching otherEntity on the Y axis
     *
     * @param primaryCollider: The primaryEntity collider
     * @param otherCollider: The otherEntity collider
     */
    return notTouchingAndAbove(primaryCollider, otherCollider) || notTouchingAndBelow(primaryCollider, otherCollider);
}

bool CollisionSystem::notTouchingAndAbove(const SDL_Rect &primaryCollider, const SDL_Rect &otherCollider) {
    /**
     * Check if the primaryEntity is above otherEntity
     *
     * @param primaryCollider: The primaryEntity collider
     * @param otherCollider: The otherEntity collider
     */
    return primaryCollider.y + primaryCollider.h < otherCollider.y;
}

bool CollisionSystem::notTouchingAndBelow(const SDL_Rect &primaryCollider, const SDL_Rect &otherCollider) {
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
//    newX += 1;  // is this needed given the collisionBuffer already accounts for this? remove if unnoticed
    primaryEntity.setTransformX(newX);
}

void CollisionSystem::handleCollisionY(Entity& primaryEntity, Entity& otherEntity) {
    /**
     * Handle primaryEntity collision with other Entities on the Y axis
     *
     * @param primaryEntity: The primary Entity
     * @param otherEntity: The other Entity
     */
    SDL_Rect primaryCollider = primaryEntity.getColliderRect();
    SDL_Rect otherCollider = otherEntity.getColliderRect();

    if (primaryCollider.y < otherCollider.y) {  // primaryEntity is above other otherEntity
        stopAndRepositionAbove(primaryEntity, primaryCollider, otherCollider);
    }
    else if (primaryCollider.y > otherCollider.y) {  // primaryEntity is below other otherEntity
        stopAndRepositionBelow(primaryEntity, otherCollider);
    }
}

void CollisionSystem::stopAndRepositionAbove(Entity& primaryEntity, const SDL_Rect& primaryCollider, const SDL_Rect& otherCollider) {
    /**
     * Stop the primaryEntity and reposition it above the other Entity
     *
     * @param primaryEntity: The primary Entity
     * @param primaryCollider: The primary Entity collider
     * @param otherCollider: The other Entity collider
     */
    auto& velocity = primaryEntity.getComponent<Velocity>();
    velocity.dy = 0;
    EventManager::getInstance().publish("groundCollision", {&primaryEntity});

    int newY = otherCollider.y - (primaryCollider.h + collisionBuffer);
//    newY += 1;  // is this necessary? Used when collisionBuffer was 2, but now it's 1. Seems to work without it. Remove if unnoticed
    primaryEntity.setTransformY(newY);
    primaryEntity.getComponent<Gravity>().gravity = primaryEntity.getComponent<Gravity>().baseGravity;
}

void CollisionSystem::stopAndRepositionBelow(Entity& primaryEntity, const SDL_Rect& otherCollider) {
    /**
     * Stop the primaryEntity and reposition it below the other other Entity
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