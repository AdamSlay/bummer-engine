#ifndef BUMMERENGINE_PHYSICSSYSTEM_H
#define BUMMERENGINE_PHYSICSSYSTEM_H

#include "../ECS/EntityManager.h"
#include "MovementSystem.h"
#include "CollisionSystem.h"


class PhysicsSystem {
public:
    void update(EntityManager& entityManager, MovementSystem& movementSystem, CollisionSystem& collisionSystem);

};


#endif //BUMMERENGINE_PHYSICSSYSTEM_H
