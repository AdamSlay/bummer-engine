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
        } else if (entity.hasComponent<Collider>() && entity.hasComponent<Position>()) {
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
        } else if (entity.hasComponent<Collider>() && entity.hasComponent<Position>()) {
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
    Position& playerPos = player.getComponent<Position>();
    Collider& playerCol = player.getComponent<Collider>();
    Scale& playerScale = player.getComponent<Scale>();

    Position& otherPos = other.getComponent<Position>();
    Collider& otherCol = other.getComponent<Collider>();
    Scale& otherScale = other.getComponent<Scale>();

    int playerX = playerPos.x + playerCol.offsetX;
    int otherX = otherPos.x + otherCol.offsetX;

    int scaledWidthPlayer = static_cast<int>(playerCol.width * playerScale.scale);
    int scaledWidthOther = static_cast<int>(otherCol.width * otherScale.scale);

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
    Position& playerPos = player.getComponent<Position>();
    Collider& playerCol = player.getComponent<Collider>();
    Scale& playerScale = player.getComponent<Scale>();

    Position& otherPos = other.getComponent<Position>();
    Collider& otherCol = other.getComponent<Collider>();
    Scale& otherScale = other.getComponent<Scale>();

    int playerY = playerPos.y + playerCol.offsetY;
    int otherY = otherPos.y + otherCol.offsetY;

    int scaledHeightPlayer = static_cast<int>(playerCol.height * playerScale.scale);
    int scaledHeightOther = static_cast<int>(otherCol.height * otherScale.scale);

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
    Position& posPlayer = player.getComponent<Position>();
    Collider& colPlayer = player.getComponent<Collider>();
    Position& posOther = other.getComponent<Position>();
    Collider& colOther = other.getComponent<Collider>();

    if (vel.dx > 0) {
        // Player is moving right
        vel.dx = 0;
        posPlayer.x = posOther.x - (collisionBuffer + colPlayer.width + colPlayer.offsetX + colOther.offsetX);
    }
    else if (vel.dx < 0) {
        // Player is moving left
        vel.dx = 0;
        posPlayer.x = collisionBuffer + colOther.offsetX + posOther.x + colOther.width;
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
    Position& posPlayer = player.getComponent<Position>();
    Collider& colPlayer = player.getComponent<Collider>();
    Position& posOther = other.getComponent<Position>();
    Collider& colOther = other.getComponent<Collider>();

    if (vel.dy > 0) {
        // Player is moving down
        vel.dy = 0;
        player.getComponent<Jumps>().jumps = 0;  // Reset the number of jumps
        posPlayer.y = posOther.y - (collisionBuffer + colPlayer.height + colPlayer.offsetY + colOther.offsetY);
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
        posPlayer.y = collisionBuffer + colOther.offsetY + posOther.y + colOther.height;
    }
}