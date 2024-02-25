#ifndef BUMMERENGINE_MOVEMENTSYSTEM_H
#define BUMMERENGINE_MOVEMENTSYSTEM_H

#include "../ECS/EntityManager.h"


class MovementSystem {
public:
    void update(EntityManager& entityManager);
    void moveX(EntityManager& entityMangaer);
    void moveY(EntityManager& entityManager);
};


#endif //BUMMERENGINE_MOVEMENTSYSTEM_H
