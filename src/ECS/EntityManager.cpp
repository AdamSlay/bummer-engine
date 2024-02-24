#include "EntityManager.h"
#include "Components.h"
#include "../Resources/TextureManager.h"

void EntityManager::setTextureManager(TextureManager* texManager) {
    /**
     * Set the texture manager for the EntityManager
     */
    this->textureManager = texManager;
}


void EntityManager::setRenderer(SDL_Renderer* ecsRenderer) {
    /**
     * Set the renderer for the EntityManager
     */
    this->renderer = ecsRenderer;
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

Entity& EntityManager::getPlayer() {
    /**
     * Return a reference to the player entity
     */
    for (Entity& entity : entities) {
        if (entity.hasComponent<Player>()) {
            return entity;
        }
    }
    throw std::runtime_error("Player not found");
}

Entity& EntityManager::createPlayer(int x, int y, int w, int h) {
    /**
     * Create a new player entity, append it to the entities vector and return a reference to it
     */
    Entity& player = createEntity();
    player.addComponent<Player>({1});
    player.addComponent<Position>({x, y});
    player.addComponent<Velocity>({0, 0});
    player.addComponent<Collider>({w, h});
    player.addComponent<Scale>({1});
    SDL_Texture* texture = textureManager->loadTexture(renderer, "assets/bb_jump_sheet.png");
    SDL_Rect srcRect = {0, 0, 64, 100};
    player.addComponent<Sprite>({texture, srcRect});
    return player;
}

Entity& EntityManager::createPlatform(int x, int y, int w, int h) {
    /**
     * Create a new platform entity, append it to the entities vector and return a reference to it
     */
    Entity& platform = createEntity();
    platform.addComponent<Position>({x, y});
    platform.addComponent<Collider>({w, h});
    SDL_Texture* texture = textureManager->loadTexture(renderer, "assets/platform.png");
    SDL_Rect srcRect = {0, 0, 256, 64};
    platform.addComponent<Sprite>({texture, srcRect});
    platform.addComponent<Scale>({1});
    return platform;
}