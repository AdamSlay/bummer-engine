#ifndef BUMMERENGINE_MOVEMENTSYSTEM_H
#define BUMMERENGINE_MOVEMENTSYSTEM_H

#include "../ECS/EntityManager.h"


class MovementSystem {
public:
    void moveX(EntityManager& entityMangaer);
    void moveY(EntityManager& entityManager);
};


#endif //BUMMERENGINE_MOVEMENTSYSTEM_H
