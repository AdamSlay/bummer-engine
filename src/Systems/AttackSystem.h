#ifndef BUMMERENGINE_ATTACKSYSTEM_H
#define BUMMERENGINE_ATTACKSYSTEM_H

#include "../ECS/EntityManager.h"

class AttackSystem {
public:
    void update(EntityManager& entityManager);
private:
    void handleInput(Entity& entity);
    void incrementAttackFrames(AttackInfo& attackInfo);
    void decrementInvincibiltyFrames(Entity& entity);
    void handleActiveAttacks(Entity& entity, EntityManager& entityManager);
    void hitOther(AttackInfo& attackInfo, Entity& other);
    bool checkCollision(SDL_Rect& hitbox, Entity& other);

};


#endif //BUMMERENGINE_ATTACKSYSTEM_H
