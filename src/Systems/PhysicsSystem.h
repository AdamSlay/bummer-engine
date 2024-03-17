#ifndef BUMMERENGINE_PHYSICSSYSTEM_H
#define BUMMERENGINE_PHYSICSSYSTEM_H

#include "../ECS/EntityManager.h"
#include "../ECS/SceneManager.h"
#include "MovementSystem.h"
#include "CollisionSystem.h"


class PhysicsSystem {
public:
    void update(SceneManager& sceneManager, EntityManager& entityManager, MovementSystem& movementSystem, CollisionSystem& collisionSystem, float deltaTime);

};


#endif //BUMMERENGINE_PHYSICSSYSTEM_H
