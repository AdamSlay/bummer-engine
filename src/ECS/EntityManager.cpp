#include "EntityManager.h"
#include "Components.h"

Entity& EntityManager::createEntity() {
    /**
     * Create a new entity, append it to the entities vector and return a reference to it
     */
    entities.emplace_back();
    return entities.back();
}

Entity& EntityManager::createPlatform(int x, int y, int w, int h) {
    /**
     * Create a new platform entity, append it to the entities vector and return a reference to it
     */
    Entity& platform = createEntity();
    platform.addComponent<Position>({x, y});
    platform.addComponent<Collider>({w, h});
    return platform;
}