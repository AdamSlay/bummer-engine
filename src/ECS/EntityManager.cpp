#include <fstream>
#include <iostream>

#include <nlohmann/json.hpp>

#include "EntityManager.h"
#include "../Utils.h"
#include "../Resources/TextureManager.h"

using json = nlohmann::json;

int Entity::nextId = 0;

Entity::Entity() {
    /**
     * Constructor for the Entity
     */
    id = nextId++;
}

bool Entity::operator==(const Entity& other) const {
    return this->id == other.id;
}

std::map<std::string, playerStates> EntityManager::playerStatesMap = {
        {"IDLE", playerStates::IDLE},
        {"GROUNDED", playerStates::GROUNDED},
        {"RUN", playerStates::RUN},
        {"JUMP_ASCEND", playerStates::JUMP_ASCEND},
        {"JUMP_APEX_ASCEND", playerStates::JUMP_APEX_ASCEND},
        {"JUMP_APEX", playerStates::JUMP_APEX},
        {"JUMP_APEX_DESCEND", playerStates::JUMP_APEX_DESCEND},
        {"JUMP_DESCEND", playerStates::JUMP_DESCEND},
        {"BASIC_ATTACK", playerStates::BASIC_ATTACK},
        {"FALL", playerStates::FALL},
        {"HIT", playerStates::HIT},
        {"STUNNED", playerStates::STUNNED},
        {"WALK", playerStates::WALK}
};

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

void EntityManager::clearEntities() {
    /**
     * Clear the entities vector
     */
    entities.clear();
}

void EntityManager::removeEntity(int entityId) {
    entities.erase(std::remove_if(entities.begin(), entities.end(), [&](const Entity& entity) {
        return entity.getID() == entityId;
    }), entities.end());
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
    /**
     * Create a new entity from a template, append it to the entities vector and return a reference to it
     *
     * @param templatePath: The path to the template file
     */
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

        if (componentsJson.contains("Player")) {
            int playerNum = componentsJson["Player"]["playerNum"];
            entity.addComponent<Player>({playerNum});
        }

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
            int x = static_cast<int>(spriteJson["srcRect"]["x"]);
            int y = static_cast<int>(spriteJson["srcRect"]["y"]);
            int w = static_cast<int>(spriteJson["srcRect"]["w"]);
            int h = static_cast<int>(spriteJson["srcRect"]["h"]);
            SDL_Rect srcRect = {x, y, w, h};
            entity.addComponent<Sprite>({texture, srcRect});
        }

        if (componentsJson.contains("State")) {
            playerStates state = playerStatesMap[componentsJson["State"]["state"]];
            entity.addComponent<State>(state);
        }

        if (componentsJson.contains("Velocity")) {
            json velocityJson = componentsJson["Velocity"];
            entity.addComponent<Velocity>({velocityJson["dx"], velocityJson["dy"], velocityJson["direction"]});
        }

        if (componentsJson.contains("Gravity")) {
            json gravityJson = componentsJson["Gravity"];
            float baseGravity = gravityJson["baseGravity"];
            float gravity = gravityJson["gravity"];
            float ascendFactor = gravityJson["ascendFactor"];
            float descendFactor = gravityJson["descendFactor"];
            entity.addComponent<Gravity>({baseGravity, gravity, ascendFactor, descendFactor});
        }

        if (componentsJson.contains("Input")) {
            std::map<SDL_Scancode, bool> keyStates;
            std::map<SDL_Scancode, bool> justPressed;
            std::map<SDL_Scancode, bool> justReleased;
            entity.addComponent<Input>({keyStates, justPressed, justReleased});
        }

        if (componentsJson.contains("Jumps")) {
            json jumpsJson = componentsJson["Jumps"];
            int jumps = jumpsJson["jumps"];
            int maxJumps = jumpsJson["maxJumps"];
            int jumpForce = jumpsJson["jumpVelocity"];
            entity.addComponent<Jumps>({jumps, maxJumps, jumpForce});
        }

        if (componentsJson.contains("Dash")) {
            json dashJson = componentsJson["Dash"];
            int speed = dashJson["speed"];
            bool isDashing = false;
            float initCooldown = dashJson["initCooldown"];
            float initDuration = dashJson["initDuration"];
            entity.addComponent<Dash>({speed, isDashing, initCooldown, initDuration});
        }

        if (componentsJson.contains("Health")) {
            json healthJson = componentsJson["Health"];
            int maxHealth = healthJson["maxHealth"];
            int currentHealth = healthJson["currentHealth"];
            int invincibilityFrames = healthJson["invincibilityFrames"];
            entity.addComponent<Health>({maxHealth, currentHealth, invincibilityFrames});
        }

        if (componentsJson.contains("AttackMap")) {
            json attackJson = componentsJson["AttackMap"];
            std::map<std::string, AttackInfo> attacks;

            for (auto& [attackName, attackPath] : attackJson.items()) {
                std::ifstream attackFile(attackPath.get<std::string>());
                if (!attackFile.is_open()) {
                    throw std::runtime_error("Could not open attack file: " + attackPath.get<std::string>());
                }
                json attackInfoJson;
                attackFile >> attackInfoJson;

                int damage = attackInfoJson["damage"];
                bool isActive = false;
                json hitboxJson = attackInfoJson["hitbox"];
                Hitbox hitbox = {hitboxJson["offsetX"], hitboxJson["offsetY"], hitboxJson["width"], hitboxJson["height"]};
                int knockback = attackInfoJson["knockback"];
                int windupframes = attackInfoJson["windupFrames"];
                int duration = attackInfoJson["duration"];
                std::cout << "added hitbox: " << hitbox.offsetX << " " << hitbox.offsetY << " " << hitbox.width << " " << hitbox.height << std::endl;
                attacks.emplace(attackName, AttackInfo{damage, isActive, hitbox, knockback, duration, windupframes});
            }

            entity.addComponent<AttackMap>({attacks});
        }

        if (componentsJson.contains("Animator")) {

            // open the animator file
            std::string animatorPath = componentsJson["Animator"]["animatorPath"];
            std::ifstream animatorFile(animatorPath);
            if (!animatorFile.is_open()) {
                throw std::runtime_error("Could not open animator file: " + componentsJson["Animator"]["animatorPath"].get<std::string>());
            }
            json animatorJson;
            animatorFile >> animatorJson;

            // iterate through the animator file and add the animations to the entity
            std::map<playerStates, AnimationClip> animations;
            for (auto& [animation, animationClips] : animatorJson["Animations"].items()) {
                playerStates state = playerStatesMap[animation];
                SDL_Texture *texture = textureManager->loadTexture(renderer, animationClips["spriteSheetPath"]);
                std::vector<SDL_Rect> frames;
                int framesPerImage = animationClips["framesPerImage"];
                int startImage = animationClips["startImage"];
                int imageCount = animationClips["imageCount"];
                int imageWidth = animationClips["imageWidth"];
                int imageHeight = animationClips["imageHeight"];
                int imageY = animationClips["imageY"];
                std::string spritePath = animationClips["spriteSheetPath"];


                for (int i = 0; i < imageCount; i++) {
                    SDL_Rect frame = {startImage * imageWidth, imageY, imageWidth, imageHeight};
                    frames.push_back(frame);
                    startImage++;
                }
                AnimationClip clip = {texture, frames, framesPerImage, true, spritePath};
                animations.emplace(state, clip);
            }

            entity.addComponent<Animator>({animations, playerStates::IDLE, 0, 0, true});
        }
        
        if (componentsJson.contains("AI")) {
            Transform& transform = entity.getComponent<Transform>();
            std::pair<int, int> patrolStart = {transform.x, transform.y};
            std::string state = componentsJson["AI"]["state"];
            float patrolRange = componentsJson["AI"]["patrolRange"];
            float attackRange = componentsJson["AI"]["attackRange"];
            entity.addComponent<AI>({state, patrolStart, patrolRange, attackRange});
        }
    }

    // Return the new entity
    return entity;
}
