#include "PhysicsSystem.h"
#include "../ECS/EventManager.h"
#include "../Config.h"

void PhysicsSystem::update(EntityManager& entityManager, MovementSystem& movementSystem, CollisionSystem& collisionSystem, float deltaTime) {
    movementSystem.moveX(entityManager, deltaTime);
    collisionSystem.updateX(entityManager);
    movementSystem.moveY(entityManager);
    collisionSystem.updateY(entityManager);

    // reset player position if it falls off the screen
    Entity& player = entityManager.getPlayer();
    if (player.getComponent<Transform>().y > SCREEN_HEIGHT) {
        player.getComponent<Transform>().y = SCREEN_HEIGHT / 4;
        player.getComponent<Transform>().x = SCREEN_WIDTH / 2;
        player.getComponent<Velocity>().dy = 0;
        EventManager::getInstance().publish("died");
        SDL_Delay(1000);
        EventManager::getInstance().publish("spawn");
    }
}
