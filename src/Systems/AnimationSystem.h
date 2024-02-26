#ifndef BUMMERENGINE_ANIMATIONSYSTEM_H
#define BUMMERENGINE_ANIMATIONSYSTEM_H

#include "../ECS/EntityManager.h"


class AnimationSystem {
public:
    void update(EntityManager& entityManager, float deltaTime);
};


#endif //BUMMERENGINE_ANIMATIONSYSTEM_H
