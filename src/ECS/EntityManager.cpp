#include <fstream>

#include <nlohmann/json.hpp>

#include "EntityManager.h"
#include "../Resources/TextureManager.h"

using json = nlohmann::json;


EntityManager::EntityManager(TextureManager* texManager, SDL_Renderer* ecsRenderer) {
    /**
     * Constructor for the EntityManager
     *
     * @param texManager: The texture manager
     * @param ecsRenderer: The SDL renderer
     */
    this->textureManager = texManager;
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

Entity& EntityManager::createEntityFromTemplate(const std::string& templatePath) {
    // Load the template file
    std::ifstream file(templatePath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open template file: " + templatePath);
    }

    // Parse the JSON
    json templateJson;
    file >> templateJson;

    // Create a new entity
    Entity& entity = createEntity();

    // For each component in the template, add the component to the entity with the specified values
    if (templateJson.contains("components")) {
        json componentsJson = templateJson["components"];

        if (componentsJson.contains("Transform")) {
            json transformJson = componentsJson["Transform"];
            entity.addComponent<Transform>({transformJson["x"], transformJson["y"], transformJson["scale"]});
        }

        if (componentsJson.contains("Collider")) {
            json colliderJson = componentsJson["Collider"];
            entity.addComponent<Collider>({colliderJson["offsetX"], colliderJson["offsetY"], colliderJson["width"], colliderJson["height"]});
        }

        if (componentsJson.contains("Sprite")) {
            json spriteJson = componentsJson["Sprite"];
            SDL_Texture* texture = textureManager->loadTexture(renderer, spriteJson["texturePath"]);
            SDL_Rect srcRect = {0, 0, 256, 32};  // You might want to include the source rect in the template
            entity.addComponent<Sprite>({texture, srcRect});
        }
    }

    // Return the new entity
    return entity;
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
    player.addComponent<Transform>({x, y, 2});
    player.addComponent<Velocity>({0, 0, 1});
    player.addComponent<Collider>({22, 55, w, h});
    player.addComponent<Gravity>({0.8, 0.8, 0.9, 1.1});
    player.addComponent<State>({playerStates::IDLE});
    player.addComponent<Jumps>({0, 2, 20});
    player.addComponent<Dash>({42, false, 0.12, 0.5});
    std::map<SDL_Scancode, bool> keyStates;
    std::map<SDL_Scancode, bool> justPressed;
    std::map<SDL_Scancode, bool> justReleased;
    player.addComponent<Input>({keyStates, justPressed, justReleased});


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

    // Add idle animation clips to the animations map
    SDL_Texture* idleTexture = textureManager->loadTexture(renderer, "assets/bb_idle_sheet.png");
    std::vector<SDL_Rect> idleFrames;
    for (int i = 0; i < 8; i++) {
        SDL_Rect frame = {i * 64, 0, 64, 100};
        idleFrames.push_back(frame);
    }
    AnimationClip idleClip = {idleTexture, idleFrames, 4, true};
    animations.emplace(playerStates::GROUNDED, idleClip);
    animations.emplace(playerStates::IDLE, idleClip);

    // Create jump state clips
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


}

Entity & EntityManager::createPlatform(int x, int y, int w, int h, float scale) {
    /**
     * Create a new platform entity, append it to the entities vector and return a reference to it
     *
     * @param x: The x position of the platform
     * @param y: The y position of the platform
     * @param w: The width of the platform
     * @param h: The height of the platform
     */
    Entity& platform = createEntity();
    int scaledW = w * scale;
    int colliderOffset = 4 * scale;
    platform.addComponent<Transform>({x, y, 1});
    platform.addComponent<Collider>({0 + colliderOffset, 0, scaledW - (colliderOffset * 2), h + colliderOffset * 4});  // hacky collider sizes
    SDL_Texture* texture = textureManager->loadTexture(renderer, "assets/platform.png");
    SDL_Rect srcRect = {0, 0, scaledW, h};
    platform.addComponent<Sprite>({texture, srcRect});
    return platform;
}

Entity& EntityManager::createMartian(int x, int y, int w, int h) {
    /**
     * Create a new martian entity, append it to the entities vector and return a reference to it
     *
     * @param x: The x position of the martian
     * @param y: The y position of the martian
     * @param w: The width of the martian
     * @param h: The height of the martian
     */
    Entity& martian = createEntity();

    std::map<playerStates, AnimationClip> animations;
    // Add idle animation clips to the animations map
    SDL_Texture* idleTexture = textureManager->loadTexture(renderer, "assets/martian_02.png");
    std::vector<SDL_Rect> idleFrames;
    for (int i = 0; i < 8; i++) {
        SDL_Rect frame = {i * 64, 0, 64, 100};
        idleFrames.push_back(frame);
    }
    AnimationClip idleClip = {idleTexture, idleFrames, 6, true};
    animations.emplace(playerStates::IDLE, idleClip);
    martian.addComponent<Animator>({animations, playerStates::IDLE, 0, 0, true});

    martian.addComponent<Transform>({x, y, 2});
    martian.addComponent<Collider>({30, 40, w, h});
    martian.addComponent<Velocity>({0, 0, 1});
    martian.addComponent<Gravity>({0.8, 0.8, 0.9, 1.1});
    martian.addComponent<State>({playerStates::IDLE});
    SDL_Rect srcRect = {0, 0, 64, 100};
    martian.addComponent<Sprite>({idleTexture, srcRect});
    return martian;
}