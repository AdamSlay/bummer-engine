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

void MovementSystem::moveY(EntityManager &entityManager) {
    /**
     * Move entities on the Y axis
     *
     * @param entityManager: The entity manager
     */
    for (Entity &entity : entityManager.getEntities()) {
        if (entity.hasComponent<Position>() && entity.hasComponent<Velocity>()) {
            Velocity &vel = entity.getComponent<Velocity>();
            if (vel.dy != 0) {
                changeJumpState(entity);
            }
            if (entity.hasComponent<Gravity>()) {
                applyGravity(entity);
            }
            // Apply velocity
            Position &pos = entity.getComponent<Position>();
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
        Jumps& jumps = entity.getComponent<Jumps>();

        if (jumps.jumps != jumps.maxJumps) {
            jumps.jumps++;
            gravity.gravity = gravity.baseGravity;
            // TODO: Magic number
            vel.dy = -15;
            entity.changeState(playerStates::JUMP_ASCEND);
        }
    }
}

void MovementSystem::applyGravity(Entity &entity) {
    /**
     * Apply gravity to the entity
     *
     * @param entity: The entity
     */
    if (entity.hasComponent<Velocity>() && entity.hasComponent<Gravity>()) {
        Velocity &vel = entity.getComponent<Velocity>();
        Gravity &gravity = entity.getComponent<Gravity>();
        if (vel.dy < 0) {
            gravity.gravity *= gravity.ascendFactor; // Decrease gravity when ascending
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
}

void MovementSystem::changeJumpState(Entity &entity) {
    /**
     * Change the jump state of the entity
     *
     * @param entity: The entity
     */
    if (entity.hasComponent<Velocity>()) {
        Velocity &vel = entity.getComponent<Velocity>();
        if (vel.dy < -3) {
            entity.changeState(playerStates::JUMP_ASCEND);
        }
        else if (vel.dy < -1) {
            entity.changeState(playerStates::JUMP_APEX_ASCEND);
        }
        else if (vel.dy < 0) {
            entity.changeState(playerStates::JUMP_APEX);
        }
        else if (0 < vel.dy && vel.dy < 2) {
            entity.changeState(playerStates::JUMP_APEX);
        }
        else if (0 < vel.dy && vel.dy < 5) {
            entity.changeState(playerStates::JUMP_APEX_DESCEND);
        }
        else if (vel.dy >= 5) {
            entity.changeState(playerStates::JUMP_DESCEND);
        }
    }
}
