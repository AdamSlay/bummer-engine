#ifndef BUMMERENGINE_MOVEMENTSYSTEM_H
#define BUMMERENGINE_MOVEMENTSYSTEM_H

#include "../ECS/EntityManager.h"


class MovementSystem {
public:
    void handleInput(EntityManager& entityManager, float deltaTime);
    void moveX(EntityManager& entityManager);
    void moveY(EntityManager& entityManager);
    void jump(Entity& entity);
    void dash(Entity& entity, float deltaTime);
    void applyGravity(Entity& entity);
    void changeJumpState(Entity& entity);
};


#endif //BUMMERENGINE_MOVEMENTSYSTEM_H
