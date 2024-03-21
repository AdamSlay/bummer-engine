#include "PhysicsSystem.h"
#include "../ECS/EventManager.h"
#include "../Config.h"

void PhysicsSystem::update(SceneManager& sceneManager, EntityManager& entityManager, MovementSystem& movementSystem, CollisionSystem& collisionSystem, float deltaTime) {
    movementSystem.handleInput(entityManager, deltaTime);
    movementSystem.moveX(entityManager);
    collisionSystem.updateX(entityManager);
    movementSystem.moveY(entityManager);
    collisionSystem.updateY(entityManager);

    // reset player position if it falls off the screen
    Entity& player = entityManager.getPlayer();
    if (player.getComponent<Transform>().y > VIRTUAL_HEIGHT) {
        player.getComponent<Transform>().y = 50;
        player.getComponent<Transform>().x = VIRTUAL_WIDTH / 2 - 20;
        player.getComponent<Velocity>().dy = 0;
        EventManager::getInstance().publish("died");
        sceneManager.nextScene();
        SDL_Delay(800);
        EventManager::getInstance().publish("spawn");
        SDL_Delay(200);
    }
}
