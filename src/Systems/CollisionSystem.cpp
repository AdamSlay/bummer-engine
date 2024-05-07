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
bool CollisionSystem::checkCollision(Entity &player, Entity &other) {
    /**
     * Check if the player is colliding with another entity
     * check
     *
     * @param player: The player entity
     * @param other: The other entity
     */
    return checkCollisionX(player, other) && checkCollisionY(player, other);
}

bool CollisionSystem::checkCollisionX(Entity &player, Entity &other) {
    /**
     * Check if the player is colliding with another entity on the X axis
     *
     * @param player: The player entity
     * @param other: The other entity
     */
    SDL_Rect playerCollider = player.getColliderRect();
    SDL_Rect otherCollider = other.getColliderRect();

    if (playerCollider.x + playerCollider.w < otherCollider.x ||  // player is left of obj
        playerCollider.x > otherCollider.x + otherCollider.w) {  // player is right of obj
        return false;
    }

    return true;
}

bool CollisionSystem::checkCollisionY(Entity &player, Entity &other) {
    /**
     * Check if the player is colliding with another entity on the Y axis
     *
     * @param player: The player entity
     * @param other: The other entity
     */
    SDL_Rect playerCollider = player.getColliderRect();
    SDL_Rect otherCollider = other.getColliderRect();

    if (playerCollider.y + playerCollider.h < otherCollider.y ||  // player is above obj
        playerCollider.y + collisionBuffer > otherCollider.y + otherCollider.h) {  // player is below obj
        return false;
    }

    return true;
}

void CollisionSystem::handleCollisionX(Entity& entity, Entity& other) {
    /**
     * Handle entity collision with other entities on the X axis
     *
     * @param entity: The entity entity
     * @param other: The other entity
     */
    auto& vel = entity.getComponent<Velocity>();
    SDL_Rect playerCollider = entity.getColliderRect();
    SDL_Rect otherCollider = other.getColliderRect();

    if (vel.dx > 0) {  // Player is moving right
        stopAndRepositionToLeft(entity, playerCollider, otherCollider);
    }
    else if (vel.dx < 0) {  // Player is moving left
        stopAndRepositionToRight(entity, playerCollider, otherCollider);
    }
}

void CollisionSystem::stopAndRepositionToLeft(Entity& entity, const SDL_Rect& playerCollider, const SDL_Rect& otherCollider) {
    /**
     * Stop the entity and reposition it to the left of the other entity
     *
     * @param entity: The entity
     * @param playerCollider: The player collider
     * @param otherCollider: The other collider
     */
    auto& velocity = entity.getComponent<Velocity>();
    velocity.dx = 0;
    int newX = otherCollider.x - (playerCollider.w + collisionBuffer);
    entity.setTransformX(newX);
}

void CollisionSystem::stopAndRepositionToRight(Entity& entity, const SDL_Rect& playerCollider, const SDL_Rect& otherCollider) {
    /**
     * Stop the entity and reposition it to the right of the other entity
     *
     * @param entity: The entity
     * @param playerCollider: The player collider
     * @param otherCollider: The other collider
     */
    auto& velocity = entity.getComponent<Velocity>();
    velocity.dx = 0;
    int newX = otherCollider.x + otherCollider.w + collisionBuffer;
//    newX += 1;  // is this needed given the collisionBuffer already accounts for this? remove if unnoticed
    entity.setTransformX(newX);
}

void CollisionSystem::handleCollisionY(Entity& entity, Entity& other) {
    /**
     * Handle entity collision with other entities on the Y axis
     *
     * @param entity: The primary entity
     * @param other: The other entity
     */
    auto& vel = entity.getComponent<Velocity>();
    SDL_Rect playerCollider = entity.getColliderRect();
    SDL_Rect otherCollider = other.getColliderRect();

    if (vel.dy > 0) {  // Player is moving down
        stopAndRepositionAbove(entity, playerCollider, otherCollider);
    }
    else if (vel.dy < 0) {  // Player is moving up
        stopAndRepositionBelow(entity, playerCollider, otherCollider);
    }
}

void CollisionSystem::stopAndRepositionAbove(Entity& entity, const SDL_Rect& playerCollider, const SDL_Rect& otherCollider) {
    /**
     * Stop the entity and reposition it above the other entity
     *
     * @param entity: The entity
     * @param playerCollider: The player collider
     * @param otherCollider: The other collider
     */
    auto& velocity = entity.getComponent<Velocity>();
    velocity.dy = 0;
    EventManager::getInstance().publish("groundCollision", {&entity});

    int newY = otherCollider.y - (playerCollider.h + collisionBuffer);
    newY += 1;  // there must be a better way to handle this, increasing collisionBuffer by 1 doesn't work
    entity.setTransformY(newY);
    entity.getComponent<Gravity>().gravity = entity.getComponent<Gravity>().baseGravity;
}

void CollisionSystem::stopAndRepositionBelow(Entity& entity, const SDL_Rect& playerCollider, const SDL_Rect& otherCollider) {
    /**
     * Stop the entity and reposition it below the other entity
     *
     * @param entity: The entity
     * @param playerCollider: The player collider
     * @param otherCollider: The other collider
     */
    auto& velocity = entity.getComponent<Velocity>();
    velocity.dy = 0;
    int newY = otherCollider.y + otherCollider.h + collisionBuffer;
    entity.setTransformY(newY);
}