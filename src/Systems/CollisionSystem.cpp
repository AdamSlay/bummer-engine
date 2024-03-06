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
    std::vector<Entity> players;
    std::vector<Entity> collisionObjects;

    // Separate the entities into two groups: players and collision objects
    for (auto & entity : entities) {
        if (entity.hasComponent<Player>()) {
            players.push_back(entity);
        } else if (entity.hasComponent<Collider>() && entity.hasComponent<Transform>()) {
            collisionObjects.push_back(entity);
        }
    }

    for (auto& player : players) {
        for (auto& other : collisionObjects) {
            if (checkCollision(player, other)) {
                if (checkCollisionX(player, other)) {
                    handlePlayerCollisionX(player, other);  // *player and *other are dereferenced Entity objects
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
    std::vector<Entity> players;
    std::vector<Entity> collisionObjects;

    // Separate the entities into two groups: players and collision objects
    for (auto & entity : entities) {
        if (entity.hasComponent<Player>()) {
            players.push_back(entity);
        } else if (entity.hasComponent<Collider>() && entity.hasComponent<Transform>()) {
            collisionObjects.push_back(entity);
        }
    }

    for (auto& player : players) {
        for (auto& other : collisionObjects) {
            if (checkCollision(player, other)) {
                if (checkCollisionY(player, other)) {
                    handlePlayerCollisionY(player, other);  // *player and *other are dereferenced Entity objects
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
    SDL_Rect playerCollider = Utils::getColliderRect(player);
    SDL_Rect otherCollider = Utils::getColliderRect(other);

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
    SDL_Rect playerCollider = Utils::getColliderRect(player);
    SDL_Rect otherCollider = Utils::getColliderRect(other);

    if (playerCollider.y + playerCollider.h < otherCollider.y ||  // player is above obj
        playerCollider.y + collisionBuffer > otherCollider.y + otherCollider.h) {  // player is below obj
        return false;
    }

    return true;
}

void CollisionSystem::handlePlayerCollisionX(Entity& player, Entity& other) {
    /**
     * Handle player collision with other entities on the X axis
     *
     * @param player: The player entity
     * @param other: The other entity
     */
    Velocity& vel = player.getComponent<Velocity>();
    SDL_Rect playerCollider = Utils::getColliderRect(player);
    SDL_Rect otherCollider = Utils::getColliderRect(other);

    if (vel.dx > 0) {
        // Player is moving right
        vel.dx = 0;
        float newPos = otherCollider.x - (playerCollider.w + collisionBuffer);
        int x = static_cast<int>(newPos);
        Utils::setTransformX(player, x);
    }
    else if (vel.dx < 0) {
        // Player is moving left
        vel.dx = 0;
        float newPos = otherCollider.x + otherCollider.w + collisionBuffer;
        int x = static_cast<int>(newPos + 1);
        Utils::setTransformX(player, x);
    }
}

void CollisionSystem::handlePlayerCollisionY(Entity& player, Entity& other) {
    /**
     * Handle player collision with other entities on the Y axis
     *
     * @param player: The player entity
     * @param other: The other entity
     */
    Velocity& vel = player.getComponent<Velocity>();
    SDL_Rect playerCollider = Utils::getColliderRect(player);
    SDL_Rect otherCollider = Utils::getColliderRect(other);

    if (vel.dy > 0) {
        // Player is moving down
        if (vel.dy > 5) {
            EventManager::getInstance().publish("landed");
        }

        vel.dy = 0;
        player.getComponent<Jumps>().jumps = 0;  // Reset the number of jumps

        float newPos = otherCollider.y - (playerCollider.h + collisionBuffer);
        int y = static_cast<int>(newPos + 1);
        Utils::setTransformY(player, y);
        if (vel.dx != 0) {
            player.changeState(playerStates::RUN);
        }
        else {
            player.changeState(playerStates::IDLE);
        }
        player.getComponent<Gravity>().gravity = player.getComponent<Gravity>().baseGravity;
    }
    else if (vel.dy < 0) {
        // Player is moving up
        vel.dy = 0;
        float newPos = otherCollider.y + otherCollider.h + collisionBuffer;
        int y = static_cast<int>(newPos + 0.5);
        Utils::setTransformY(player, y);
    }
}