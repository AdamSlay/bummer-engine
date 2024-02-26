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

    int scaledWidthPlayer = static_cast<int>(playerCol.width * playerScale.scale);
    int scaledWidthOther = static_cast<int>(otherCol.width * otherScale.scale);

    if (playerPos.x + scaledWidthPlayer < otherPos.x ||  // player is left of obj
        playerPos.x > otherPos.x + scaledWidthOther) {  // player is right of obj
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

    int scaledHeightPlayer = static_cast<int>(playerCol.height * playerScale.scale);
    int scaledHeightOther = static_cast<int>(otherCol.height * otherScale.scale);

    if (playerPos.y + scaledHeightPlayer < otherPos.y ||  // player is above obj
        playerPos.y > otherPos.y + scaledHeightOther) {  // player is below obj
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
    Position& posOther = other.getComponent<Position>();

    if (vel.dx > 0) {
        // Player is moving right
        vel.dx = 0;
        int playerWidth = static_cast<int>(player.getComponent<Collider>().width * player.getComponent<Scale>().scale);
        posPlayer.x = posOther.x - (playerWidth + collisionBuffer);  // Move the player to the left of the platform
    }
    else if (vel.dx < 0) {
        // Player is moving left
        vel.dx = 0;
        posPlayer.x = collisionBuffer + posOther.x + other.getComponent<Collider>().width * other.getComponent<Scale>().scale;  // Move the player to the right of the platform
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
    Position& posOther = other.getComponent<Position>();

    if (vel.dy > 0) {
        // Player is moving down
        vel.dy = 0;
        player.getComponent<JUMPS>().jumps = 0;  // Reset the number of jumps
        int playerHeight = static_cast<int>(player.getComponent<Collider>().height * player.getComponent<Scale>().scale);
        posPlayer.y = posOther.y - (playerHeight + collisionBuffer);  // Move the player to the top of the platform
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
        posPlayer.y = collisionBuffer + posOther.y + other.getComponent<Collider>().height * other.getComponent<Scale>().scale;  // Move the player to the bottom of the platform
    }
}