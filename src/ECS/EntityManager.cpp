#include "EntityManager.h"
#include "../Resources/TextureManager.h"

void EntityManager::setTextureManager(TextureManager* texManager) {
    /**
     * Set the texture manager for the EntityManager
     *
     * @param texManager: The texture manager
     */
    this->textureManager = texManager;
}


void EntityManager::setRenderer(SDL_Renderer* ecsRenderer) {
    /**
     * Set the renderer for the EntityManager
     *
     * @param ecsRenderer: The SDL renderer
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
     *
     * @param x: The x position of the player
     * @param y: The y position of the player
     * @param w: The width of the player
     * @param h: The height of the player
     */
    Entity& player = createEntity();

    std::map<playerStates, AnimationClip> animations;
    configureAnimator(player, animations);
    player.addComponent<Animator>({animations, playerStates::IDLE, 0, 0, true});
    player.addComponent<Player>({1});
    player.addComponent<Transform>({x, y, 1.5});
    player.addComponent<Velocity>({0, 0, 1});
    player.addComponent<Collider>({22, 55, w, h});
    player.addComponent<Gravity>({0.8, 0.8, 0.9, 1.1});
    player.addComponent<State>({playerStates::IDLE});
    player.addComponent<Jumps>({0, 2});
    std::map<SDL_Scancode, bool> keyMap;
    player.addComponent<Input>(keyMap);


    SDL_Texture* texture = textureManager->loadTexture(renderer, "assets/bb_jump_sheet.png");
    SDL_Rect srcRect = {0, 0, 64, 100};
    player.addComponent<Sprite>({texture, srcRect});
    return player;
}

void EntityManager::configureAnimator(Entity& entity, std::map<playerStates, AnimationClip>& animations) {
    /**
     * Configure the animator for the entity
     *
     * @param entity: The entity
     */
    // Add run animation clips to the animations map
    SDL_Texture* runTexture = textureManager->loadTexture(renderer, "assets/bb_run_sheet.png");
    std::vector<SDL_Rect> runFrames;
    for (int i = 0; i < 8; i++) {
        SDL_Rect frame = {i * 64, 0, 64, 100};
        runFrames.push_back(frame);
    }
    AnimationClip runClip = {runTexture, runFrames, 4, true};
    animations.emplace(playerStates::RUN, runClip);


    // Add jump animation clips to the animations map
    SDL_Texture* jumpTexture = textureManager->loadTexture(renderer, "assets/bb_jump_sheet.png");
    std::vector<SDL_Rect> jumpFrames;
    for (int i = 2; i < 8; i++) {
        SDL_Rect frame = {i * 64, 0, 64, 100};
        jumpFrames.push_back(frame);
    }
    SDL_Rect jumpAscend = jumpFrames[0];
    AnimationClip jumpAscendClip = {jumpTexture, {jumpAscend}, 1, false};
    animations.emplace(playerStates::JUMP_ASCEND, jumpAscendClip);

    SDL_Rect jumpAscendApex = jumpFrames[1];
    AnimationClip jumpAscendApexClip = {jumpTexture, {jumpAscendApex}, 1, false};
    animations.emplace(playerStates::JUMP_APEX_ASCEND, jumpAscendApexClip);

    SDL_Rect jumpApex = jumpFrames[2];
    AnimationClip jumpApexClip = {jumpTexture, {jumpApex}, 1, false};
    animations.emplace(playerStates::JUMP_APEX, jumpApexClip);

    SDL_Rect jumpDescendApex = jumpFrames[3];
    AnimationClip jumpDescendApexClip = {jumpTexture, {jumpDescendApex}, 1, false};
    animations.emplace(playerStates::JUMP_APEX_DESCEND, jumpDescendApexClip);

    SDL_Rect jumpDescend = jumpFrames[4];
    AnimationClip jumpDescendClip = {jumpTexture, {jumpDescend}, 1, false};
    animations.emplace(playerStates::JUMP_DESCEND, jumpDescendClip);


    // Add idle animation clips to the animations map
    AnimationClip idleClip = {jumpTexture, {{0, 0, 64, 100}}, 4, true};
    animations.emplace(playerStates::IDLE, idleClip);
    animations.emplace(playerStates::GROUNDED, idleClip);
}

Entity& EntityManager::createPlatform(int x, int y, int w, int h) {
    /**
     * Create a new platform entity, append it to the entities vector and return a reference to it
     *
     * @param x: The x position of the platform
     * @param y: The y position of the platform
     * @param w: The width of the platform
     * @param h: The height of the platform
     */
    Entity& platform = createEntity();
    platform.addComponent<Transform>({x, y, 1});
    platform.addComponent<Collider>({10, 0, w - 18, h});
    SDL_Texture* texture = textureManager->loadTexture(renderer, "assets/platform.png");
    SDL_Rect srcRect = {0, 0, w, h};
    platform.addComponent<Sprite>({texture, srcRect});
    return platform;
}