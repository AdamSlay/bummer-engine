#include "AttackSystem.h"
#include "../Utils.h"
#include "../ECS/EventManager.h"

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
            handleInput(entity);
            handleActiveAttacks(entity, entityManager);
        }
    }
    for (int entityId : entitiesToRemove) {
        entityManager.removeEntity(entityId);
    }
    entitiesToRemove.clear();
}

void AttackSystem::handleInput(Entity& entity) {
    /**
     * Handle input for the attack system
     * If the attack button is pressed, set the basic attack to active
     *
     * @param entity: The entity
     */

    if (entity.hasComponent<Input>() && entity.hasComponent<AttackMap>()) {
        Input& input = entity.getComponent<Input>();
        AttackMap& attackMap = entity.getComponent<AttackMap>();

        if (input.justPressed[SDL_SCANCODE_R]) {
            attackMap.attacks["basic"].isActive = true;
            if (entity.hasComponent<Player>()) {
                // suspend player on attack
                Velocity& vel = entity.getComponent<Velocity>();
                vel.dy = 0;
                vel.dx = 0;
                Gravity& gravity = entity.getComponent<Gravity>();
                gravity.gravity = 0.5;
            }
            entity.changeState(playerStates::BASIC_ATTACK);
            Utils::publishEvent("basicAttack", &entity);
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

    for (auto& [name, attackInfo] : attacker.getComponent<AttackMap>().attacks) {
        if (attackInfo.isActive) {
            // Could the issue with the knockback involve the enemy having an active attack at the time that it gets hit/stunned?
            State& state = attacker.getComponent<State>();
            if (state.state == playerStates::STUNNED || state.state == playerStates::HIT) {
                attackInfo.isActive = false;
                break;
            }
            incrementAttackFrames(attackInfo);

            SDL_Rect hitbox = Utils::getHitboxRect(attackInfo.hitbox, attacker);
            for (Entity& other : entityManager.getEntities()) {
                int attackFrame = attackInfo.frameCounter;
                int windupFrames = attackInfo.windupFrames;
                if (checkCollision(hitbox, other) && other.hasComponent<Health>() && attackFrame >= windupFrames) {
                    hitOther(attackInfo, attacker, other);
                }
            }
        }
        else if (attacker.getComponent<State>().state == playerStates::BASIC_ATTACK) {
            Velocity& vel = attacker.getComponent<Velocity>();
            if (vel.dx == 0) {
                attacker.changeState(playerStates::IDLE);
            }
            else {
                attacker.changeState(playerStates::RUN);
            }
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
    std::cout << "attack frame: " << attackInfo.frameCounter << std::endl;
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

    Health& otherHealth = other.getComponent<Health>();
    if (otherHealth.invincibilityRemaining == 0) {
        // Set state to hit
        State& otherState = other.getComponent<State>();
        if (otherState.state != playerStates::HIT) {
            other.changeState(playerStates::HIT);
        }

        // Publish enemyHit event
        Utils::publishEvent("enemyHit", &attacker, &other);

        // Reduce health
        std::cout << "Health: " << otherHealth.currentHealth << std::endl;
        otherHealth.currentHealth -= attackInfo.damage;
        otherHealth.invincibilityRemaining = otherHealth.invincibilityFrames;

        // Apply knockback
        Transform& attackerTransform = attacker.getComponent<Transform>();
        Transform& otherTransform = other.getComponent<Transform>();
        Velocity& otherVel = other.getComponent<Velocity>();
        int knockbackDirection = (attackerTransform.x < otherTransform.x) ? 1 : -1;
        otherVel.dx = attackInfo.knockback * knockbackDirection;
        std::cout << "knocking back with: " << otherVel.dx << std::endl;
        otherVel.direction = knockbackDirection * -1;

        if (otherHealth.currentHealth <= 0) {
            if (!other.hasComponent<Player>()) {
                entitiesToRemove.push_back(other.getID());
            }
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
        Health& health = entity.getComponent<Health>();

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

    SDL_Rect otherCollider = Utils::getColliderRect(other);

    if (hitbox.x + hitbox.w < otherCollider.x ||  // hitbox is left of obj
        hitbox.x > otherCollider.x + otherCollider.w ||  // hitbox is right of obj
        hitbox.y + hitbox.h < otherCollider.y ||  // hitbox is above obj
        hitbox.y > otherCollider.y + otherCollider.h) {  // hitbox is below obj
        return false;
    }
    return true;
}