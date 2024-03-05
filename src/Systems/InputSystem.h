#ifndef BUMMERENGINE_INPUTSYSTEM_H
#define BUMMERENGINE_INPUTSYSTEM_H

#include "../ECS/EntityManager.h"

class InputSystem {
public:
    void update(EntityManager& entityManager, bool& quit);
};

#endif //BUMMERENGINE_INPUTSYSTEM_H