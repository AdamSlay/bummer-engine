#include "AISystem.h"

#include <iostream>

#include "../ECS/EventManager.h"
#include "../Utils.h"


void AISystem::update(EntityManager& entityManager) {
    for (Entity& entity : entityManager.getEntities()) {
        if (entity.hasComponent<Npc>()) {
            entity.resetIntent(true);
            patrol(entity);
            attack(entityManager, entity);
        }
    }
}

void AISystem::patrol(Entity& entity) {
    AI& ai = entity.getComponent<AI>();
    Transform& transform = entity.getComponent<Transform>();
    Velocity& velocity = entity.getComponent<Velocity>();

    // If the enemy is at the edge of the patrol range, reverse direction
    int patrolDirection = velocity.direction;
    bool atLeftEdge = transform.x <= ai.patrolStart.first - ai.patrolRange;
    bool atRightEdge = transform.x >= ai.patrolStart.first + ai.patrolRange;
    if ((patrolDirection == 1 && atRightEdge) || (patrolDirection == -1 && atLeftEdge)) {
        patrolDirection *= -1;
    }

    // Move the enemy in the current direction
    Intent& intent = entity.getComponent<Intent>();
    if (patrolDirection == 1) {
        intent.action = Action::MOVE_RIGHT;
        intent.direction = Direction::RIGHT;
    }
    else {
        intent.action = Action::MOVE_LEFT;
        intent.direction = Direction::LEFT;
    }
}

void AISystem::attack(EntityManager& entityManager, Entity& attacker) {
    if (attacker.hasComponent<AttackMap>()) {
        for (Entity& entity : entityManager.getEntities()) {
            if (entity.hasComponent<Player>()) {
                Entity& player = entity;
                Transform& playerTransform = player.getComponent<Transform>();
                AI& ai = attacker.getComponent<AI>();
                Transform& aiTransform = attacker.getComponent<Transform>();

                float distance = std::sqrt(std::pow(playerTransform.x - aiTransform.x, 2) + std::pow(playerTransform.y - aiTransform.y, 2));
                if (distance <= ai.attackRange) {
                    // Player is within attack range, trigger attack
                    Intent& intent = attacker.getComponent<Intent>();
                    intent.action = Action::ATTACK;
                    if (aiTransform.x < playerTransform.x && attacker.getComponent<Velocity>().direction == -1) {
                        attacker.getComponent<Velocity>().direction = 1;
                    }
                    else if (aiTransform.x > playerTransform.x && attacker.getComponent<Velocity>().direction == 1) {
                        attacker.getComponent<Velocity>().direction = -1;
                    }
                }
                else if (distance <= ai.pursuitRange) {
                    // Player is within patrol range, move towards player
                    Intent& intent = attacker.getComponent<Intent>();
                    // if player is on a higher platform, ignore them
                    if (playerTransform.y < aiTransform.y - 50) {
                        continue;
                    }

                    if (aiTransform.x < playerTransform.x) {
                        intent.action = Action::MOVE_RIGHT;
                        intent.direction = Direction::RIGHT;
                    }
                    else {
                        intent.action = Action::MOVE_LEFT;
                        intent.direction = Direction::LEFT;
                    }
                }
            }
        }
    }
}