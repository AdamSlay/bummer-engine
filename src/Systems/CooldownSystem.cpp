#include "CooldownSystem.h"


void CooldownSystem::update(EntityManager &entityManager, float deltaTime) {
    for (auto &entity : entityManager.getEntities()) {
        if (entity.hasComponent<Dash>()) {
            Dash& dash = entity.getComponent<Dash>();
            if (dash.currentCooldown > 0) {
                dash.currentCooldown -= deltaTime;
                if (dash.currentCooldown < 0) {
                    dash.currentCooldown = 0;
                }
            }
        }
    }
}