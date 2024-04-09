#pragma once
#ifndef BUMMERENGINE_STATEMACHINE_H
#define BUMMERENGINE_STATEMACHINE_H

#include "EntityManager.h"

class StateMachine {
public:
StateMachine(EntityManager& entityManager);
static bool canMove(Entity& entity);
private:
    EntityManager entityManager;
};


#endif //BUMMERENGINE_STATEMACHINE_H
