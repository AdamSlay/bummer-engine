#include "PhysicsSystem.h"
#include "../ECS/EventManager.h"
#include "../Config.h"

void PhysicsSystem::update(SceneManager& sceneManager, EntityManager& entityManager, MovementSystem& movementSystem, CollisionSystem& collisionSystem, float deltaTime) {
    movementSystem.handleIntent(entityManager, deltaTime);
    movementSystem.moveX(entityManager);
    movementSystem.moveY(entityManager);
    collisionSystem.update(entityManager);

    // reset player position if it falls off the screen
    Entity player = entityManager.getPlayer();
    if (player.getComponent<Transform>().y > VIRTUAL_HEIGHT) {
        player.getComponent<Transform>().y = 50;
        player.getComponent<Transform>().x = VIRTUAL_WIDTH / 2 - 20;
        player.getComponent<Velocity>().dy = 0;
        EventManager::getInstance().publish("died", {&player});
        sceneManager.nextScene();
        SDL_Delay(800);
        EventManager::getInstance().publish("spawn", {&player});
        SDL_Delay(200);
    }
}
