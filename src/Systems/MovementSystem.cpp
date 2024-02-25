#include "MovementSystem.h"

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
