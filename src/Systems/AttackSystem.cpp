#include "AttackSystem.h"
#include "../Utils.h"
#include "../ECS/EventManager.h"
#include "./CollisionSystem.h"

#include <iostream>


void AttackSystem::update(EntityManager& entityManager) {
    /**
     * Update the attack system
     * For each entity, decrement invincibility frames and handle active attacks
     *
     * @param entityManager: The entity manager
     */

    for (Entity& entity : entityManager.getEntities()) {
        decrementInvincibiltyFrames(entity);

        if (entity.hasComponent<AttackMap>()) {
            handleIntent(entity);
            handleActiveAttacks(entity, entityManager);
        }
    }
    for (int entityId : entitiesToRemove) {
        entityManager.removeEntity(entityId);
    }
    entitiesToRemove.clear();
}

void AttackSystem::handleIntent(Entity& entity) {
    /**
     * Handle input for the attack system
     * If the attack button is pressed, set the basic attack to active
     *
     * @param entity: The entity
     */
    // TODO: this function is too long and should be broken up into smaller functions
    if (entity.hasComponent<Intent>() && entity.hasComponent<AttackMap>()) {
        auto& intent = entity.getComponent<Intent>();
        auto& attackMap = entity.getComponent<AttackMap>();

        if (intent.action == Action::ATTACK) {
            // publish event to StateMachine
            EventManager::getInstance().publish("basicAttack", {&entity});

            // if state is basic attack, set attack to active
            auto& state = entity.getComponent<State>();
            if (state.state == playerStates::BASIC_ATTACK) {
                // TODO: AttackSystem::setBasicAttackActive() or similar
                attackMap.attacks["basic"].isActive = true;
                auto& vel = entity.getComponent<Velocity>();
                auto& gravity = entity.getComponent<Gravity>();
                vel.dy = 0;
                vel.dx = 0;
                gravity.gravity = 0.5;
            }
        }
    }
}

void AttackSystem::handleActiveAttacks(Entity &attacker, EntityManager &entityManager) {
    /**
     * Handle active attacks
     * If an attack is active, increment the frame counter and check for collision
     * If a collision is detected, reduce the health of the other entity by the attack's damage
     * and set the other entity's invincibility frames
     *
     * @param entity: The entity
     * @param entityManager: The entity manager
     */
     // TODO: This function is too long and should be broken up into smaller functions
    for (auto& [name, attackInfo] : attacker.getComponent<AttackMap>().attacks) {
        if (attackInfo.isActive) {
            // Could the issue with the knockback involve the enemy having an active attack at the time that it gets hit/stunned?
            auto& state = attacker.getComponent<State>();
            if (state.state == playerStates::STUNNED || state.state == playerStates::HIT) {
                attackInfo.isActive = false;
                break;
            }
            incrementAttackFrames(attackInfo);

            //
            SDL_Rect hitbox = attacker.getHitboxRect(attackInfo.hitbox);
            for (Entity& other : entityManager.getEntities()) {

                // TODO: this is a lot of logic to have in a conditional statement. Could this be broken up?

                // if they are colliding && the other has health && the attack has exhausted its windup frames(therefore it's active)
                int attackFrame = attackInfo.frameCounter;
                int windupFrames = attackInfo.windupFrames;
                if (checkCollision(hitbox, other) && other.hasComponent<Health>() && attackFrame >= windupFrames) {
                    hitOther(attackInfo, attacker, other);
                }
            }
        }
        else {
            EventManager::getInstance().publish("attackEnd", {&attacker});
        }
    }
}

void AttackSystem::incrementAttackFrames(AttackInfo& attackInfo) {
    /**
     * Increment the attack frames
     *
     * @param attackInfo: The attack info
     */

    attackInfo.frameCounter += 1;
    if (attackInfo.frameCounter == attackInfo.duration) {
        attackInfo.isActive = false;
        attackInfo.frameCounter = 0;
    }
}

void AttackSystem::hitOther(AttackInfo& attackInfo, Entity& attacker, Entity& other) {
    /**
     * Reduce the health of the other entity by the attack's damage
     * and set the other entity's invincibility frames
     *
     * @param attackInfo: The attack info
     * @param other: The other entity
     */
    auto& otherHealth = other.getComponent<Health>();
    if (otherHealth.invincibilityRemaining == 0) {

        EventManager::getInstance().publish("enemyHit", {&attacker, &other});

        // TODO: AttackSystem should not be responsible for applying knockback. That's what the MovementSystem is for.
        // TODO: AttackSystem should not be responsible for reducing health. That's what the HealthSystem is for.
        applyKnockback(attackInfo, attacker, other);
        reduceHealth(attackInfo, other);
    }
}

void AttackSystem::applyKnockback(AttackInfo& attackInfo, Entity& attacker, Entity& other) {
    /**
     * Apply knockback to the other entity based on the attack's knockback
     *
     * @param attackInfo: The attack info
     * @param other: The other entity
     */

    // TODO: the AttackSystem shouldn't be responsible for 'moving' any entities. That's what the MovementSystem is for.

    auto& attackerTransform = attacker.getComponent<Transform>();
    auto& otherTransform = other.getComponent<Transform>();
    auto& otherVel = other.getComponent<Velocity>();
    int knockbackDirection = (attackerTransform.x < otherTransform.x) ? 1 : -1;
    otherVel.dx = attackInfo.knockback * knockbackDirection;
    otherVel.direction = knockbackDirection * -1;
}

void AttackSystem::reduceHealth(AttackInfo& attackInfo, Entity& entity) {
    /**
     * Reduce the health of an entity based on the attack's damage
     *
     * If the entity's health is less than or equal to 0, add the entity to the list of entities to remove
     *
     * @param attackInfo: The attack info
     * @param entity: The entity
     */

     // TODO: should the AttackSystem be responsible for reducing health? Could that be a job for the HealthSystem.

    auto& health = entity.getComponent<Health>();
    health.currentHealth -= attackInfo.damage;
    health.invincibilityRemaining = health.invincibilityFrames;

    if (health.currentHealth <= 0) {
        if (!entity.hasComponent<Player>()) {
            entitiesToRemove.push_back(entity.getID());
        }
    }
}

void AttackSystem::decrementInvincibiltyFrames(Entity& entity) {
    /**
     * Decrement the invincibility frames of an entity
     *
     * @param entity: The entity
     */

    if (entity.hasComponent<Health>()) {
        auto& health = entity.getComponent<Health>();

        // TODO: AttackSystem should not be responsible for changing the state of an entity. That's what the StateMachine is for.

        if (health.invincibilityRemaining > 0) {
            entity.changeState(playerStates::STUNNED);
            health.invincibilityRemaining -= 1;
            if (health.invincibilityRemaining == 0) {
                entity.changeState(playerStates::IDLE);
            }
        }
    }
}

bool AttackSystem::checkCollision(SDL_Rect& hitbox, Entity& other) {
    /**
     * Check if the hitbox is colliding with another entity
     *
     * @param hitbox: The hitbox
     * @param entity2: The other entity
     */
    SDL_Rect otherCollider = other.getColliderRect();

    if (CollisionSystem::isTouchingXaxis(hitbox, otherCollider) &&
            CollisionSystem::isTouchingYaxis(hitbox, otherCollider)) {
        return true;
    }
    return false;
}