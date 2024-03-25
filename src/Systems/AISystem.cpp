#include "AISystem.h"

#include <iostream>

#include "../ECS/EventManager.h"
#include "../Utils.h"


void AISystem::update(EntityManager& entityManager) {
    for (Entity& entity : entityManager.getEntities()) {
        if (entity.hasComponent<Npc>()) {
            // clear action intent
            entity.resetIntent();
            State& state = entity.getComponent<State>();
            if (state.state != playerStates::BASIC_ATTACK && state.state != playerStates::HIT && state.state != playerStates::STUNNED) {
                AI& ai = entity.getComponent<AI>();
                Transform& transform = entity.getComponent<Transform>();
                Velocity& velocity = entity.getComponent<Velocity>();

                // If the enemy is at the edge of the patrol range, reverse direction
                if ((velocity.direction == 1 && transform.x >= ai.patrolStart.first + ai.patrolRange) ||
                    (velocity.direction == -1 && transform.x <= ai.patrolStart.first - ai.patrolRange)) {
                    velocity.direction *= -1;
                }

                // Move the enemy in the current direction
                entity.changeState(playerStates::RUN);
                velocity.dx = 1 * velocity.direction;
                if (entity.hasComponent<AttackMap>()) {
                    attack(entityManager, entity);
                }
            }
        }
    }
}

void AISystem::attack(EntityManager& entityManager, Entity& attacker) {
    for (Entity& entity : entityManager.getEntities()) {
        if (entity.hasComponent<Player>()) {
            Entity& player = entity;
            Transform& playerTransform = player.getComponent<Transform>();
            AI& ai = attacker.getComponent<AI>();
            Transform& aiTransform = attacker.getComponent<Transform>();

            float distance = std::sqrt(std::pow(playerTransform.x - aiTransform.x, 2) + std::pow(playerTransform.y - aiTransform.y, 2));
            if (distance <= ai.attackRange) {
                // Player is within attack range, trigger attack
                // face player if not already facing
                if (aiTransform.x < playerTransform.x && attacker.getComponent<Velocity>().direction == -1) {
                    attacker.getComponent<Velocity>().direction = 1;
                }
                else if (aiTransform.x > playerTransform.x && attacker.getComponent<Velocity>().direction == 1) {
                    attacker.getComponent<Velocity>().direction = -1;
                }

                Intent& intent = attacker.getComponent<Intent>();
                intent.action = Action::ATTACK;
            }
        }
    }
}