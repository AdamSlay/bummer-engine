#pragma once
#ifndef BUMMERENGINE_COLLISIONSYSTEM_H
#define BUMMERENGINE_COLLISIONSYSTEM_H

#include "../ECS/EntityManager.h"
#include "../ECS/Components.h"


class CollisionSystem {
public:
    void updateX(EntityManager& entityManager);
    void updateY(EntityManager& entityManager);
    bool checkCollision(Entity& primaryEntity, Entity& otherEntity);
    bool checkCollisionX(Entity& primaryEntity, Entity& otherEntity);
    bool checkCollisionY(Entity& primaryEntity, Entity& otherEntity);
    void handleCollisionX(Entity& primaryEntity, Entity& otherEntity);
    void handleCollisionY(Entity& primaryEntity, Entity& otherEntity);
    void stopAndRepositionToLeft(Entity& primaryEntity, const SDL_Rect& primaryCollider, const SDL_Rect& otherCollider);
    void stopAndRepositionToRight(Entity& primaryEntity, const SDL_Rect& otherCollider);
    void stopAndRepositionAbove(Entity& primaryEntity, const SDL_Rect& primaryCollider, const SDL_Rect& otherCollider);
    void stopAndRepositionBelow(Entity& primaryEntity, const SDL_Rect& otherCollider);

private:
    int collisionBuffer = 1;
};


#endif //BUMMERENGINE_COLLISIONSYSTEM_H
