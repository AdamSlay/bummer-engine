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
            int entityId = data.primaryEntity->id;
            Entity& entity = entityManager.getEntityById(entityId);
            State& state = entity.getComponent<State>();
            Velocity& vel = entity.getComponent<Velocity>();

            if (entity.hasComponent<Jumps>()) {
                entity.getComponent<Jumps>().jumps = 0;  // Reset the number of jumps
            }

            // if the player was falling, play the landed sound
            if (state.state == playerState::JUMP_ASCEND ||
                state.state == playerState::JUMP_DESCEND ||
                state.state == playerState::JUMP_APEX ||
                state.state == playerState::JUMP_APEX_DESCEND ||
                state.state == playerState::JUMP_APEX_ASCEND ||
                state.state == playerState::FLYING) {
                EventManager::getInstance().publish("landed", {data.primaryEntity});
            }

            // The entity is now grounded
            entity.changeFlyingState(false);

            // change the entity's state based on its velocity
            if (vel.dx != 0 && state.state != playerState::STUNNED && state.state != playerState::BASIC_ATTACK && state.state != playerState::DASHING) {
                // moving side to side, not stunned or attacking
                if (state.state != playerState::RUN) {
                    entity.changeState(playerState::RUN);
                }
            }
            else if (state.state != playerState::STUNNED && state.state != playerState::BASIC_ATTACK && state.state != playerState::DASHING) {
                // not moving, not stunned or attacking
                if (state.state != playerState::IDLE) {
                    entity.changeState(playerState::IDLE);
                }
            }

        }
        catch (const std::runtime_error& e) {
            std::cout << "Could not find entity with id: " << data.primaryEntity->id << std::endl;

        }

    });

    EventManager::getInstance().subscribe("moveLeft", [&](EventData data) {
        Entity* entity = data.primaryEntity;
        State& state = entity->getComponent<State>();
        if (state.isFlying) {
           state.state = playerState::FLYING;
        }
        else {
            state.state = playerState::RUN;
        }
    });

    EventManager::getInstance().subscribe("moveRight", [&](EventData data) {
        Entity* entity = data.primaryEntity;
        State& state = entity->getComponent<State>();
        if (state.isFlying) {
           state.state = playerState::FLYING;
        }
        else {
            state.state = playerState::RUN;
        }
    });

    EventManager::getInstance().subscribe("idle", [&](EventData data) {
        Entity* entity = data.primaryEntity;
        State& state = entity->getComponent<State>();
        if (state.isFlying) {
           state.state = playerState::FLYING;
        }
        else {
            state.state = playerState::IDLE;
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
            if (state.state != playerState::STUNNED && state.state != playerState::HIT && state.state != playerState::BASIC_ATTACK && state.state != playerState::DASHING) {
                // not stunned or hit or currently attacking, can attack
                entity->changeState(playerState::BASIC_ATTACK);
                EventManager::getInstance().publish("basicAttackSound", {entity});
            }
        }
        catch (const std::runtime_error& e) {
            std::cout << "Could not find entity with id: " << data.primaryEntity->id << std::endl;
        }
    });

    EventManager::getInstance().subscribe("attackEnd", [&](EventData data) {
        /**
         * Handle the basic attack end event
         *
         * @param data: The event data
         */
        try {
            Entity* entity = data.primaryEntity;
            auto& state = entity->getComponent<State>();
            if (state.state == playerState::BASIC_ATTACK) {
                auto& vel = entity->getComponent<Velocity>();
                if (vel.dx == 0) {
                    entity->changeState(playerState::IDLE);
                }
                else {
                    entity->changeState(playerState::RUN);
                }
            }
        }
        catch (const std::runtime_error& e) {
            std::cout << "Could not find entity with id: " << data.primaryEntity->id << std::endl;
        }
    });

    EventManager::getInstance().subscribe("enemyHit", [&](EventData data) {
        /**
         * Handle the enemy hit event
         *
         * @param data: The event data
         */
        try {
            Entity* other = data.secondaryEntity;
            auto& state = other->getComponent<State>();
            if (state.state != playerState::HIT) {
                other->changeState(playerState::HIT);
            }
        }
        catch (const std::runtime_error& e) {
            std::cout << "Could not find entity with id: " << data.primaryEntity->id << std::endl;
        }
    });

    EventManager::getInstance().subscribe("dash", [&](EventData data) {
        try {
            Entity* entity = data.primaryEntity;
            entity->changeState(playerState::DASHING);
            EventManager::getInstance().publish("dashSound", {entity});
        }
        catch (const std::runtime_error& e) {
            std::cout << "Could not find entity with id: " << data.primaryEntity->id << std::endl;
        }
    });

    EventManager::getInstance().subscribe("dashEnd", [&](EventData data) {
        try {
            Entity* entity = data.primaryEntity;
            entity->changeState(playerState::IDLE);
        }
        catch (const std::runtime_error& e) {
            std::cout << "Could not find entity with id: " << data.primaryEntity->id << std::endl;
        }
    });

    EventManager::getInstance().subscribe("jump", [&](EventData data) {
        try {
            Entity* entity = data.primaryEntity;
            entity->changeState(playerState::FLYING);
            EventManager::getInstance().publish("jumpSound", {entity});

        }
        catch (const std::runtime_error& e) {
            std::cout << "Could not find entity with id: " << data.primaryEntity->id << std::endl;
        }
    });

    EventManager::getInstance().subscribe("airborne", [&](EventData data) {
        try {
            Entity* entity = data.primaryEntity;

            if (entity->hasComponent<Velocity>() && entity->getComponent<State>().state != playerState::BASIC_ATTACK) {
                Velocity &vel = entity->getComponent<Velocity>();
                entity->changeFlyingState(true);
                entity->changeState(playerState::FLYING);
                // if (vel.dy < -3) {
                //     entity->changeState(playerState::JUMP_ASCEND);
                // }
                // else if (vel.dy < -1) {
                //     entity->changeState(playerState::JUMP_APEX_ASCEND);
                // }
                // else if (0 < vel.dy && vel.dy < 2) {
                //     entity->changeState(playerState::JUMP_APEX);
                // }
                // else if (0 < vel.dy && vel.dy < 5) {
                //     entity->changeState(playerState::JUMP_APEX_DESCEND);
                // }
                // else if (vel.dy >= 5) {
                //     entity->changeState(playerState::JUMP_DESCEND);
                // }
            }
        }
        catch (const std::runtime_error& e) {
            std::cout << "Could not find entity with id: " << data.primaryEntity->id << std::endl;
        }
    });
    EventManager::getInstance().subscribe("runLeft", [&](EventData data) {
        try {
            Entity* entity = data.primaryEntity;
            if (entity->getComponent<State>().state != playerState::BASIC_ATTACK) {
                entity->changeState(playerState::RUN);
            }
        }
        catch (const std::runtime_error& e) {
            std::cout << "Could not find entity with id: " << data.primaryEntity->id << std::endl;
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
    bool notStunned = state.state != playerState::STUNNED;
    bool notHit = state.state != playerState::HIT;
    bool notAttacking = state.state != playerState::BASIC_ATTACK;
    bool notDashing = state.state != playerState::DASHING;

    return notStunned && notHit && notAttacking && notDashing;
}