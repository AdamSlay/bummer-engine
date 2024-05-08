#include "CollisionSystem.h"
#include "../ECS/Components.h"
#include "../ECS/EventManager.h"
#include "../Utils.h"

#include <iostream>


void CollisionSystem::updateX(EntityManager& entityManager) {
    /**
     * Update the X axis collision
     *
     * @param entityManager: The entity manager
     */
    auto& entities = entityManager.getEntities();
    for (auto& entity : entities) {
        if (entity.hasComponent<Player>() || entity.hasComponent<Npc>()) {
            for (auto& other : entities) {
                if (entity.id != other.id && checkCollision(entity, other)) {
                    handleCollisionX(entity, other);  // *player and *other are dereferenced Entity objects
                }
            }
        }
    }
}

void CollisionSystem::updateY(EntityManager& entityManager) {
    /**
     * Update the Y axis collision
     *
     * @param entityManager: The entity manager
     */
    auto& entities = entityManager.getEntities();
    for (auto& entity : entities) {
        if (entity.hasComponent<Player>() || entity.hasComponent<Npc>()) {
            for (auto& other : entities) {
                if (entity.id != other.id && checkCollision(entity, other)) {
                    handleCollisionY(entity, other);  // *player and *other are dereferenced Entity objects
                }
            }
        }
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

    if (primaryCollider.x + primaryCollider.w < otherCollider.x ||  // player is left of obj
        primaryCollider.x > otherCollider.x + otherCollider.w) {  // player is right of obj
        return false;
    }

    return true;
}

bool CollisionSystem::checkCollisionY(Entity &primaryEntity, Entity &otherEntity) {
    /**
     * Check if the primaryEntity is colliding with another Entity on the Y axis
     *
     * @param primaryEntity: The primary Entity
     * @param otherEntity: The other Entity
     */
    SDL_Rect primaryCollider = primaryEntity.getColliderRect();
    SDL_Rect otherCollider = otherEntity.getColliderRect();

    if (primaryCollider.y + primaryCollider.h < otherCollider.y ||  // player is above obj
        primaryCollider.y + collisionBuffer > otherCollider.y + otherCollider.h) {  // player is below obj
        return false;
    }

    return true;
}

void CollisionSystem::handleCollisionX(Entity& primaryEntity, Entity& otherEntity) {
    /**
     * Handle primaryEntity collision with other Entities on the X axis
     *
     * @param entity: The primaryEntity primaryEntity
     * @param other: The otherEntity primaryEntity
     */
    auto& velocity = primaryEntity.getComponent<Velocity>();
    SDL_Rect primaryCollider = primaryEntity.getColliderRect();
    SDL_Rect otherCollider = otherEntity.getColliderRect();

    if (velocity.dx > 0) {  // Player is moving right
        stopAndRepositionToLeft(primaryEntity, primaryCollider, otherCollider);
    }
    else if (velocity.dx < 0) {  // Player is moving left
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
    auto& velocity = primaryEntity.getComponent<Velocity>();
    SDL_Rect primaryCollider = primaryEntity.getColliderRect();
    SDL_Rect otherCollider = otherEntity.getColliderRect();

    if (velocity.dy > 0) {  // Player is moving down
        stopAndRepositionAbove(primaryEntity, primaryCollider, otherCollider);
    }
    else if (velocity.dy < 0) {  // Player is moving up
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