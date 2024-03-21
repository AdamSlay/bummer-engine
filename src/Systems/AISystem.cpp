#include "AISystem.h"
#include <iostream>


void AISystem::update(EntityManager& entityManager) {
    for (Entity& entity : entityManager.getEntities()) {
        if (entity.hasComponent<AI>() && entity.hasComponent<Transform>() && entity.hasComponent<Velocity>()) {
            AI& ai = entity.getComponent<AI>();
            Transform& transform = entity.getComponent<Transform>();
            Velocity& velocity = entity.getComponent<Velocity>();

            // If the enemy is at the edge of the patrol range, reverse direction
            if ((velocity.direction == 1 && transform.x >= ai.patrolStart.first + ai.patrolRange) ||
                (velocity.direction == -1 && transform.x <= ai.patrolStart.first - ai.patrolRange)) {
                velocity.direction *= -1;
            }

            // Move the enemy in the current direction
            State& state = entity.getComponent<State>();
            if (state.state != playerStates::WALK) {
                entity.changeState(playerStates::WALK);
            }
            velocity.dx = 1 * velocity.direction;
        }
    }
}