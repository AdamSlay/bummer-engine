#include "CollisionSystem.h"
#include "../ECS/Components.h"


void CollisionSystem::update(EntityManager& entityManager) {
    auto& entities = entityManager.getEntities();
    for (auto entityA = entities.begin(); entityA != entities.end(); ++entityA) {
        if (entityA->hasComponent<Position>() && entityA->hasComponent<Collider>()) {
            Position& posA = entityA->getComponent<Position>();
            Collider& colA = entityA->getComponent<Collider>();

            for (auto entityB = entityA + 1; entityB != entities.end(); ++entityB) {
                if (entityB->hasComponent<Position>() && entityB->hasComponent<Collider>()) {
                    Position& posB = entityB->getComponent<Position>();
                    Collider& colB = entityB->getComponent<Collider>();

                    if (checkCollision(*entityA, *entityB)) {
                        if (entityA->hasComponent<Player>()) {
                            handlePlayerCollision(*entityA, *entityB);  // *entityA and *entityB are dereferenced Entity objects
                        } else if (entityB->hasComponent<Player>()) {
                            handlePlayerCollision(*entityB, *entityA);
                        }
                    }
                }
            }
        }
    }
}

bool CollisionSystem::checkCollision(Entity &entityA, Entity &entityB) {
    Position& posA = entityA.getComponent<Position>();
    Collider& colA = entityA.getComponent<Collider>();
    Scale& scaleA = entityA.getComponent<Scale>();

    Position& posB = entityB.getComponent<Position>();
    Collider& colB = entityB.getComponent<Collider>();
    Scale& scaleB = entityB.getComponent<Scale>();

    int scaledWidthA = colA.width * scaleA.scale;
    int scaledHeightA = colA.height * scaleA.scale;
    int scaledWidthB = colB.width * scaleB.scale;
    int scaledHeightB = colB.height * scaleB.scale;

    if (posA.x + scaledWidthA < posB.x ||  // A is left of B
        posA.y + scaledHeightA < posB.y ||  // A is above B
        posA.x > posB.x + scaledWidthB ||  // A is right of B
        posA.y > posB.y + scaledHeightB) {  // A is below B
        return false;
    }

    // If none of the above are true, the rectangles are intersecting
    return true;
}

void CollisionSystem::handlePlayerCollision(Entity& player, Entity& other) {
    Velocity& vel = player.getComponent<Velocity>();
    Position& posPlayer = player.getComponent<Position>();
    Position& posOther = other.getComponent<Position>();
    Collider& colOther = other.getComponent<Collider>();

    if (vel.dy > 0 && posPlayer.y + vel.dy > posOther.y - colOther.height) {
        // Player is moving downwards and will be inside the platform on the next frame
        vel.dy = 0;
        posPlayer.y = posOther.y - colOther.height;  // Move the player to the top of the platform
    }
}