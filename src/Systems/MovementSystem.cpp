#include "MovementSystem.h"
#include <iostream>

#include "../ECS/Components.h"

void MovementSystem::moveX(EntityManager& entityManager) {
    /**
     * Move entities on the X axis
     *
     * @param entityManager: The entity manager
     */
    for (Entity& entity : entityManager.getEntities()) {
        if (entity.hasComponent<Position>() && entity.hasComponent<Velocity>()) {
            Position& pos = entity.getComponent<Position>();
            Velocity& vel = entity.getComponent<Velocity>();
            pos.x += vel.dx;
            if (vel.dx != 0) {
                vel.direction = (vel.dx > 0) ? 1 : -1;
            }
        }
    }
}

void MovementSystem::moveY(EntityManager& entityManager) {
    /**
     * Move entities on the Y axis
     *
     * @param entityManager: The entity manager
     */
    for (Entity& entity : entityManager.getEntities()) {
        if (entity.hasComponent<Position>() && entity.hasComponent<Velocity>()) {
            Position& pos = entity.getComponent<Position>();
            Velocity& vel = entity.getComponent<Velocity>();
            if (entity.hasComponent<Gravity>()) {
                // Apply gravity scaling
                Gravity& gravity = entity.getComponent<Gravity>();
                if (vel.dy < 0) {
                    gravity.gravity *= gravity.ascendFactor; // Decrease gravity when ascending
                    // TODO: Magic number
                    if (gravity.gravity < 0.65) {
                        gravity.gravity = 0.65;
                    }
                }
                else {
                    gravity.gravity *= gravity.descendFactor; // Increase gravity when descending
                }
                // Apply gravity
                vel.dy += gravity.gravity;
            }
            // Apply velocity
            pos.y += vel.dy;
        }
    }
}

void MovementSystem::jump(Entity& entity) {
    /**
     * Make the entity jump
     *
     * @param entity: The entity
     */
    if (entity.hasComponent<Velocity>() && entity.hasComponent<State>()) {
        Velocity& vel = entity.getComponent<Velocity>();
        State& state = entity.getComponent<State>();
        Gravity& gravity = entity.getComponent<Gravity>();
        Animator& animator = entity.getComponent<Animator>();

        if (state.state != playerStates::DOUBLE_JUMP && state.state != playerStates::JUMP) {
            gravity.gravity = gravity.baseGravity;
            // TODO: Magic number
            vel.dy = -20;
            state.changeState(playerStates::JUMP, animator);
        }
        else if (state.state == playerStates::JUMP) {
            gravity.gravity = gravity.baseGravity;
            // TODO: Magic number
            vel.dy = -20;
            state.changeState(playerStates::DOUBLE_JUMP, animator);
        }
    }
}
