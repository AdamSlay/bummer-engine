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
    Transform& playerPos = player.getComponent<Transform>();
    Collider& playerCol = player.getComponent<Collider>();
    Scale& playerScale = player.getComponent<Scale>();

    Transform& otherPos = other.getComponent<Transform>();
    Collider& otherCol = other.getComponent<Collider>();
    Scale& otherScale = other.getComponent<Scale>();

    float playerX = playerPos.x + playerCol.offsetX * playerScale.scale;
    float otherX = otherPos.x + otherCol.offsetX * otherScale.scale;

    float scaledWidthPlayer = playerCol.width * playerScale.scale;
    float scaledWidthOther = otherCol.width * otherScale.scale;

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
    Transform& playerPos = player.getComponent<Transform>();
    Collider& playerCol = player.getComponent<Collider>();
    Scale& playerScale = player.getComponent<Scale>();

    Transform& otherPos = other.getComponent<Transform>();
    Collider& otherCol = other.getComponent<Collider>();
    Scale& otherScale = other.getComponent<Scale>();

    float playerY = playerPos.y + playerCol.offsetY * playerScale.scale;
    float otherY = otherPos.y + otherCol.offsetY * otherScale.scale;

    float scaledHeightPlayer = playerCol.height * playerScale.scale;
    float scaledHeightOther = otherCol.height * otherScale.scale;

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
    Transform& posPlayer = player.getComponent<Transform>();
    Collider& colPlayer = player.getComponent<Collider>();
    Scale& playerScale = player.getComponent<Scale>();
    Transform& posOther = other.getComponent<Transform>();
    Collider& colOther = other.getComponent<Collider>();
    Scale& otherScale = other.getComponent<Scale>();

    if (vel.dx > 0) {
        // Player is moving right
        vel.dx = 0;
        float newPos = (posOther.x  + (colOther.offsetX * otherScale.scale)) - (collisionBuffer + (colPlayer.width * playerScale.scale) + (colPlayer.offsetX * playerScale.scale));
        posPlayer.x = static_cast<int>(newPos + 0.5);
    }
    else if (vel.dx < 0) {
        // Player is moving left
        vel.dx = 0;
        float newPos = posOther.x + (colOther.width * otherScale.scale) + (colOther.offsetX * otherScale.scale) + collisionBuffer - (colPlayer.offsetX * playerScale.scale);
        posPlayer.x = static_cast<int>(newPos + 0.5);
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
    Transform& posPlayer = player.getComponent<Transform>();
    Collider& colPlayer = player.getComponent<Collider>();
    Scale& playerScale = player.getComponent<Scale>();
    Transform& posOther = other.getComponent<Transform>();
    Collider& colOther = other.getComponent<Collider>();
    Scale& otherScale = other.getComponent<Scale>();

    if (vel.dy > 0) {
        // Player is moving down
        vel.dy = 0;
        player.getComponent<Jumps>().jumps = 0;  // Reset the number of jumps
        
        float newPos = posOther.y - (collisionBuffer + (colPlayer.height * playerScale.scale) + colPlayer.offsetY * playerScale.scale + colOther.offsetY * otherScale.scale);
        posPlayer.y = static_cast<int>(newPos + 0.5);
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
        float newPos = posOther.y + (colOther.height * otherScale.scale) + (colOther.offsetY * otherScale.scale) + collisionBuffer - (colPlayer.offsetY * playerScale.scale);
        posPlayer.y = static_cast<int>(newPos + 0.5);
    }
}