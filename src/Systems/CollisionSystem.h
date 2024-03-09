#ifndef BUMMERENGINE_COLLISIONSYSTEM_H
#define BUMMERENGINE_COLLISIONSYSTEM_H

#include "../ECS/EntityManager.h"
#include "../ECS/Components.h"


class CollisionSystem {
public:
    void updateX(EntityManager& entityManager);
    void updateY(EntityManager& entityManager);
    bool checkCollision(Entity& player, Entity& other);
    bool checkCollisionX(Entity& player, Entity& other);
    bool checkCollisionY(Entity& player, Entity& other);
    void handlePlayerCollisionX(Entity& player, Entity& other);
    void handlePlayerCollisionY(Entity& player, Entity& other);

private:
    int collisionBuffer = 2;
};


#endif //BUMMERENGINE_COLLISIONSYSTEM_H
