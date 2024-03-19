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
            if (!attackMap.attacks["basic"].isActive) {
                attackMap.attacks["basic"].isActive = true;
                if (entity.hasComponent<Player>()) {
                    // suspend player on attack
                    Velocity& vel = entity.getComponent<Velocity>();
                    vel.dy = 0;
                    vel.dx = 0;
                    Gravity& gravity = entity.getComponent<Gravity>();
                    gravity.gravity = 0.5;
                }
                EventManager::getInstance().publish("basicAttack");
            }
        }
    }
}

void AttackSystem::handleActiveAttacks(Entity &entity, EntityManager &entityManager) {
    /**
     * Handle active attacks
     * If an attack is active, increment the frame counter and check for collision
     * If a collision is detected, reduce the health of the other entity by the attack's damage
     * and set the other entity's invincibility frames
     *
     * @param entity: The entity
     * @param entityManager: The entity manager
     */

    for (auto& [name, attackInfo] : entity.getComponent<AttackMap>().attacks) {
        if (attackInfo.isActive) {
            incrementAttackFrames(attackInfo);

            SDL_Rect hitbox = Utils::getHitboxRect(attackInfo.hitbox, entity);
            for (Entity& other : entityManager.getEntities()) {
                if (checkCollision(hitbox, other) && other.hasComponent<Health>()) {
                    hitOther(attackInfo, other);
                }
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
    if (attackInfo.frameCounter == attackInfo.duration) {
        attackInfo.isActive = false;
        attackInfo.frameCounter = 0;
    }
}

void AttackSystem::hitOther(AttackInfo& attackInfo, Entity& other) {
    /**
     * Reduce the health of the other entity by the attack's damage
     * and set the other entity's invincibility frames
     *
     * @param attackInfo: The attack info
     * @param other: The other entity
     */

    Health& otherHealth = other.getComponent<Health>();
    if (otherHealth.invincibilityRemaining == 0) {
        otherHealth.currentHealth -= attackInfo.damage;
        otherHealth.invincibilityRemaining = otherHealth.invincibilityFrames;
        std::cout << "Health: " << otherHealth.currentHealth << std::endl;
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
            health.invincibilityRemaining -= 1;
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