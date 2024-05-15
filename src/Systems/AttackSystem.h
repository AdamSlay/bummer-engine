#pragma once
#ifndef BUMMERENGINE_ATTACKSYSTEM_H
#define BUMMERENGINE_ATTACKSYSTEM_H

#include "../ECS/EntityManager.h"

class AttackSystem {
public:
    void update(EntityManager& entityManager);
private:
    // entities to remove vector
    std::vector<int> entitiesToRemove;
    void handleIntent(Entity& entity);
    void incrementAttackFrames(AttackInfo& attackInfo);
    void decrementInvincibiltyFrames(Entity& entity);
    void handleActiveAttacks(Entity& entity, EntityManager& entityManager);
    void hitOther(AttackInfo& attackInfo, Entity& attacker, Entity& other);
    void applyKnockback(AttackInfo& attackInfo, Entity& attacker, Entity& other);
    bool checkCollision(SDL_Rect& hitbox, Entity& other);

};


#endif //BUMMERENGINE_ATTACKSYSTEM_H
