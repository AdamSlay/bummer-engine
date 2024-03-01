#include "CollisionSystem.h"
#include "../ECS/Components.h"

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
    Transform& playerTrans = player.getComponent<Transform>();
    Collider& playerCol = player.getComponent<Collider>();

    Transform& otherTrans = other.getComponent<Transform>();
    Collider& otherCol = other.getComponent<Collider>();

    float playerX = playerTrans.x + playerCol.offsetX * playerTrans.scale;
    float otherX = otherTrans.x + otherCol.offsetX * otherTrans.scale;

    float scaledWidthPlayer = playerCol.width * playerTrans.scale;
    float scaledWidthOther = otherCol.width * otherTrans.scale;

    if (playerX + scaledWidthPlayer < otherX ||  // player is left of obj
        playerX > otherX + scaledWidthOther) {  // player is right of obj
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
    Transform& playerTrans = player.getComponent<Transform>();
    Collider& playerCol = player.getComponent<Collider>();

    Transform& otherTrans = other.getComponent<Transform>();
    Collider& otherCol = other.getComponent<Collider>();

    float playerY = playerTrans.y + playerCol.offsetY * playerTrans.scale;
    float otherY = otherTrans.y + otherCol.offsetY * otherTrans.scale;

    float scaledHeightPlayer = playerCol.height * playerTrans.scale;
    float scaledHeightOther = otherCol.height * otherTrans.scale;

    if (playerY + scaledHeightPlayer < otherY ||  // player is above obj
        playerY > otherY + scaledHeightOther) {  // player is below obj
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
    Transform& playerTrans = player.getComponent<Transform>();
    Collider& colPlayer = player.getComponent<Collider>();

    Transform& otherTrans = other.getComponent<Transform>();
    Collider& colOther = other.getComponent<Collider>();

    if (vel.dx > 0) {
        // Player is moving right
        vel.dx = 0;
        float newPos = (otherTrans.x + (colOther.offsetX * otherTrans.scale)) - (collisionBuffer + (colPlayer.width * playerTrans.scale) + (colPlayer.offsetX * playerTrans.scale));
        playerTrans.x = static_cast<int>(newPos + 0.5);
    }
    else if (vel.dx < 0) {
        // Player is moving left
        vel.dx = 0;
        float newPos = otherTrans.x + (colOther.width * otherTrans.scale) + (colOther.offsetX * otherTrans.scale) + collisionBuffer - (colPlayer.offsetX * playerTrans.scale);
        playerTrans.x = static_cast<int>(newPos + 0.5);
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
    Transform& playerTrans = player.getComponent<Transform>();
    Collider& colPlayer = player.getComponent<Collider>();

    Transform& otherTrans = other.getComponent<Transform>();
    Collider& colOther = other.getComponent<Collider>();

    if (vel.dy > 0) {
        // Player is moving down
        vel.dy = 0;
        player.getComponent<Jumps>().jumps = 0;  // Reset the number of jumps
        
        float newPos = otherTrans.y - (collisionBuffer + (colPlayer.height * playerTrans.scale) + colPlayer.offsetY * playerTrans.scale + colOther.offsetY * otherTrans.scale);
        playerTrans.y = static_cast<int>(newPos + 0.5);
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
        float newPos = otherTrans.y + (colOther.height * otherTrans.scale) + (colOther.offsetY * otherTrans.scale) + collisionBuffer - (colPlayer.offsetY * playerTrans.scale);
        playerTrans.y = static_cast<int>(newPos + 0.5);
    }
}