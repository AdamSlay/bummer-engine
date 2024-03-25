#ifndef BUMMERENGINE_COOLDOWNSYSTEM_H
#define BUMMERENGINE_COOLDOWNSYSTEM_H

#include "../ECS/EntityManager.h"

class CooldownSystem {
public:
    void update(EntityManager &entityManager, float deltaTim);

};


#endif //BUMMERENGINE_COOLDOWNSYSTEM_H
