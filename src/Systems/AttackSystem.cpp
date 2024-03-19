#include "AttackSystem.h"
#include "../Utils.h"

#include <iostream>


void AttackSystem::update(EntityManager& entityManager) {

    // Decrement invincibility frames
    for (Entity& entity : entityManager.getEntities()) {
        if (entity.hasComponent<Health>()) {
            Health& health = entity.getComponent<Health>();
            if (health.invincibilityRemaining > 0) {
                health.invincibilityRemaining -= 1;
            }
        }
    }

    // Check for active attacks
    for (Entity& entity : entityManager.getEntities()) {
        if (entity.hasComponent<AttackMap>()) {
            for (auto& [name, attackInfo] : entity.getComponent<AttackMap>().attacks) {
                if (attackInfo.isActive) {
                    // if attack is active, increment frame counter and check for collision
                    std::cout << "Attack is active" << std::endl;
                    attackInfo.frameCounter += 1;
                    if (attackInfo.frameCounter == attackInfo.duration) {
                        attackInfo.isActive = false;
                        attackInfo.frameCounter = 0;
                    }
                    // Check for collision and reduce health
                    SDL_Rect hitbox = Utils::getHitboxRect(attackInfo.hitbox, entity);
                    for (Entity& other : entityManager.getEntities()) {
                        if (other.hasComponent<Health>() && checkCollision(hitbox, other)) {
                            Health& otherHealth = other.getComponent<Health>();
                            if (otherHealth.invincibilityRemaining > 0) {
                                continue;
                            }
                            else {
                                other.getComponent<Health>().currentHealth -= attackInfo.damage;
                                otherHealth.invincibilityRemaining = otherHealth.invincibilityFrames;
                                std::cout << "Health: " << other.getComponent<Health>().currentHealth << std::endl;
                            }
                        }
                    }
                }
            }
        }
    }
}

bool AttackSystem::checkCollision(SDL_Rect& hitbox, Entity& entity2) {
    SDL_Rect otherCollider = Utils::getColliderRect(entity2);

    if (hitbox.x + hitbox.w < otherCollider.x ||  // hitbox is left of obj
        hitbox.x > otherCollider.x + otherCollider.w ||  // hitbox is right of obj
        hitbox.y + hitbox.h < otherCollider.y ||  // hitbox is above obj
        hitbox.y > otherCollider.y + otherCollider.h) {  // hitbox is below obj
        return false;
    }

    return true;
}