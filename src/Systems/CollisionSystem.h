#ifndef BUMMERENGINE_COLLISIONSYSTEM_H
#define BUMMERENGINE_COLLISIONSYSTEM_H

#include "../ECS/EntityManager.h"
#include "../ECS/Components.h"


class CollisionSystem {
public:
    void update(EntityManager& entityManager);
    bool checkCollision(Entity& entityA, Entity& entityB);
    void handlePlayerCollision(Entity& player, Entity& other);
};


#endif //BUMMERENGINE_COLLISIONSYSTEM_H
