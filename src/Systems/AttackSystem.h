#ifndef BUMMERENGINE_ATTACKSYSTEM_H
#define BUMMERENGINE_ATTACKSYSTEM_H

#include "../ECS/EntityManager.h"

class AttackSystem {
public:
    void update(EntityManager& entityManager);
private:
    bool checkCollision(SDL_Rect& hitbox, Entity& entity2);

};


#endif //BUMMERENGINE_ATTACKSYSTEM_H
