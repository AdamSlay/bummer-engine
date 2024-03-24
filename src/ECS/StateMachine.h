#ifndef BUMMERENGINE_STATEMACHINE_H
#define BUMMERENGINE_STATEMACHINE_H

#include "EntityManager.h"

class StateMachine {
public:
StateMachine(EntityManager& entityManager);
private:
    EntityManager entityManager;
};


#endif //BUMMERENGINE_STATEMACHINE_H
