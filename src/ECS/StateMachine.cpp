#include "StateMachine.h"

#include <iostream>

#include "EventManager.h"
#include "../Utils.h"

StateMachine::StateMachine(EntityManager& entityManager) : entityManager(entityManager) {

    EventManager::getInstance().subscribe("groundCollision", [&](EventData data) {
        /**
         * Handle the ground collision event
         *
         * @param data: The event data
         */
        try {
            Entity& entity = entityManager.getEntityById(data.entityId);
            State& state = entity.getComponent<State>();
            Velocity& vel = entity.getComponent<Velocity>();

            // if the player was falling, play the landed sound
            if (state.state == playerStates::JUMP_ASCEND ||
                state.state == playerStates::JUMP_DESCEND ||
                state.state == playerStates::JUMP_APEX ||
                state.state == playerStates::JUMP_APEX_DESCEND ||
                state.state == playerStates::JUMP_APEX_ASCEND) {
                EventManager::getInstance().publish("landed", {data.entityId});
            }

            // change the entity's state based on its velocity
            if (vel.dx != 0 && state.state != playerStates::STUNNED && state.state != playerStates::BASIC_ATTACK) {
                // moving side to side, not stunned or attacking
                entity.changeState(playerStates::RUN);
            }
            else if (state.state != playerStates::STUNNED && state.state != playerStates::BASIC_ATTACK) {
                // not moving, not stunned or attacking
                entity.changeState(playerStates::IDLE);
            }

        }
        catch (const std::runtime_error& e) {
            std::cout << "Could not find entity with id: " << data.entityId << std::endl;

        }

    });


    EventManager::getInstance().subscribe("basicAttack", [&](EventData data) {
        /**
         * Handle the basic attack event
         *
         * @param data: The event data
         */

        try {
            Entity* entity = data.primaryEntity;
            State& state = entity->getComponent<State>();
            if (state.state != playerStates::STUNNED && state.state != playerStates::HIT) {
                // not stunned or hit, can attack
                entity->changeState(playerStates::BASIC_ATTACK);
                Utils::publishEvent("basicAttackSound", entity);
            }
        }
        catch (const std::runtime_error& e) {
            std::cout << "Could not find entity with id: " << data.entityId << std::endl;
        }
    });
}