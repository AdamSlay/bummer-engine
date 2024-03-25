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
            if (vel.dx != 0 && state.state != playerStates::STUNNED && state.state != playerStates::BASIC_ATTACK && state.state != playerStates::DASHING) {
                // moving side to side, not stunned or attacking
                entity.changeState(playerStates::RUN);
            }
            else if (state.state != playerStates::STUNNED && state.state != playerStates::BASIC_ATTACK && state.state != playerStates::DASHING) {
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
            if (state.state != playerStates::STUNNED && state.state != playerStates::HIT && state.state != playerStates::BASIC_ATTACK && state.state != playerStates::DASHING) {
                // not stunned or hit or currently attacking, can attack
                entity->changeState(playerStates::BASIC_ATTACK);
                Utils::publishEvent("basicAttackSound", entity);
            }
        }
        catch (const std::runtime_error& e) {
            std::cout << "Could not find entity with id: " << data.entityId << std::endl;
        }
    });

    EventManager::getInstance().subscribe("dash", [&](EventData data) {
        try {
            Entity* entity = data.primaryEntity;
            entity->changeState(playerStates::DASHING);
            Utils::publishEvent("dashSound", entity);
        }
        catch (const std::runtime_error& e) {
            std::cout << "Could not find entity with id: " << data.entityId << std::endl;
        }
    });

    EventManager::getInstance().subscribe("dashEnd", [&](EventData data) {
        try {
            Entity* entity = data.primaryEntity;
            entity->changeState(playerStates::IDLE);
        }
        catch (const std::runtime_error& e) {
            std::cout << "Could not find entity with id: " << data.entityId << std::endl;
        }
    });
}

bool StateMachine::canMove(Entity &entity) {
    /**
     * Check if the entity can move
     *
     * @param entity: The entity
     * @return: True if the entity can move, false otherwise
     */
    State& state = entity.getComponent<State>();
    bool notStunned = state.state != playerStates::STUNNED;
    bool notHit = state.state != playerStates::HIT;
    bool notAttacking = state.state != playerStates::BASIC_ATTACK;
    bool notDashing = state.state != playerStates::DASHING;

    return notStunned && notHit && notAttacking && notDashing;
}