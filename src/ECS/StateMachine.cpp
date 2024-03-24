#include "StateMachine.h"

#include <iostream>

#include "EventManager.h"

StateMachine::StateMachine(EntityManager& entityManager) : entityManager(entityManager) {
    EventManager::getInstance().subscribe("groundCollision", [&](EventData data) {
        try{
            Entity& entity = entityManager.getEntityById(data.entityId);
            State& state = entity.getComponent<State>();
            if (state.state == playerStates::JUMP_ASCEND ||
                state.state == playerStates::JUMP_DESCEND ||
                state.state == playerStates::JUMP_APEX ||
                state.state == playerStates::JUMP_APEX_DESCEND ||
                state.state == playerStates::JUMP_APEX_ASCEND) {
                // Transition to appropriate state, e.g., IDLE
                state.state = playerStates::IDLE;
                EventManager::getInstance().publish("landed", {data.entityId});
            }
        }
        catch (const std::runtime_error& e) {
            std::cout << "Could not find entity with id: " << data.entityId << std::endl;

        }

    });
}