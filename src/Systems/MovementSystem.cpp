#include "MovementSystem.h"
#include <iostream>

#include "../ECS/Components.h"

void MovementSystem::moveX(EntityManager& entityManager) {
    for (Entity& entity : entityManager.getEntities()) {
        if (entity.hasComponent<Position>() && entity.hasComponent<Velocity>()) {
            Position& pos = entity.getComponent<Position>();
            Velocity& vel = entity.getComponent<Velocity>();
            pos.x += vel.dx;
        }
    }
}

void MovementSystem::moveY(EntityManager& entityManager) {
    for (Entity& entity : entityManager.getEntities()) {
        if (entity.hasComponent<Position>() && entity.hasComponent<Velocity>()) {
            Position& pos = entity.getComponent<Position>();
            Velocity& vel = entity.getComponent<Velocity>();
            if (entity.hasComponent<Gravity>()) {
                Gravity& gravity = entity.getComponent<Gravity>();
                vel.dy += gravity.gravity;
            }
            pos.y += vel.dy;
        }
    }
}

void MovementSystem::jump(Entity& entity) {
    if (entity.hasComponent<Velocity>() && entity.hasComponent<State>()) {
        Velocity& vel = entity.getComponent<Velocity>();
        State& state = entity.getComponent<State>();
        if (state.state != playerStates::DOUBLE_JUMP && state.state != playerStates::JUMP) {
            vel.dy = -15;
            std::cout << "State is " << static_cast<int>(state.state) << std::endl;
            state.state = playerStates::JUMP;
        }
        else if (state.state == playerStates::JUMP) {
            vel.dy = -15;
            state.state = playerStates::DOUBLE_JUMP;
        }
    }
}
