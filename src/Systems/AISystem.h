#ifndef BUMMERENGINE_AISYSTEM_H
#define BUMMERENGINE_AISYSTEM_H

#include "../ECS/EntityManager.h"
#include "../Systems/MovementSystem.h"

class AISystem {
public:
    void update(EntityManager& entityManager);
    void attack(EntityManager& entityManager, Entity& attacker);
};


#endif //BUMMERENGINE_AISYSTEM_H
