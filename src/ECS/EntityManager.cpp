#include "EntityManager.h"
#include "Components.h"
#include "../Resources/TextureManager.h"

void EntityManager::setTextureManager(TextureManager* tm) {
    /**
     * Set the texture manager for the EntityManager
     */
    this->textureManager = tm;
}


void EntityManager::setRenderer(SDL_Renderer* renderer) {
    /**
     * Set the renderer for the EntityManager
     */
    this->renderer = renderer;
}


Entity& EntityManager::createEntity() {
    /**
     * Create a new entity, append it to the entities vector and return a reference to it
     */
    entities.emplace_back();
    return entities.back();
}

std::vector<Entity>& EntityManager::getEntities() {
    /**
     * Return a reference to the entities vector
     */
    return entities;
}

Entity& EntityManager::createPlatform(int x, int y, int w, int h) {
    /**
     * Create a new platform entity, append it to the entities vector and return a reference to it
     */
    Entity& platform = createEntity();
    platform.addComponent<Position>({x, y});
    platform.addComponent<Collider>({w, h});
    SDL_Texture* texture = textureManager->loadTexture(renderer, "assets/platform.png");
    platform.addComponent<Sprite>({texture});
    return platform;
}