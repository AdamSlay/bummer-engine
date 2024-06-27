#include <fstream>
#include <iostream>

#include "EntityManager.h"
#include "../Utils.h"

using json = nlohmann::json;

std::map<std::string, playerState> EntityManager::playerStatesMap = {
        {"IDLE",              playerState::IDLE},
        {"GROUNDED",          playerState::GROUNDED},
        {"RUN",               playerState::RUN},
        {"JUMP_ASCEND",       playerState::JUMP_ASCEND},
        {"JUMP_DESCEND",      playerState::JUMP_DESCEND},
        {"JUMP_APEX",         playerState::JUMP_APEX},
        {"JUMP_APEX_DESCEND", playerState::JUMP_APEX_DESCEND},
        {"JUMP_APEX_ASCEND",  playerState::JUMP_APEX_ASCEND},
        {"BASIC_ATTACK",      playerState::BASIC_ATTACK},
        {"HIT",               playerState::HIT},
        {"STUNNED",           playerState::STUNNED},
        {"DASHING",           playerState::DASHING}
};

EntityManager::EntityManager(TextureManager* textureManager, SDL_Renderer* renderer) {
    /**
     * Constructor for the EntityManager
     *
     * @param texManager: The texture manager
     * @param ecsRenderer: The SDL renderer
     */
    this->textureManager = textureManager;
    this->renderer = renderer;
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
    Entity newEntity = Entity(entities.size());
    entities.emplace_back(newEntity);
    return entities.back();
}

std::vector<Entity>& EntityManager::getEntities() {
    /**
     * Return a reference to the entities vector
     */
    return entities;
}

Entity& EntityManager::getEntityById(int id) {
    /**
     * Get an entity by its id
     */
    for (Entity& entity : entities) {
        if (entity.getID() == id) {
            return entity;
        }
    }
    throw std::runtime_error("Entity with ID " + std::to_string(id) + " not found.");
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

std::vector<Entity>& EntityManager::getCollidableEntities() {
    collidableEntities.clear();
    for (auto& entity : entities) {
        if (entity.hasComponent<Collider>()) {
            collidableEntities.push_back(entity);
        }
    }
    return collidableEntities;
}

std::vector<Entity>& EntityManager::getMovableCollidableEntities() {
    movableCollidableEntities.clear();
    for (auto& entity : entities) {
        if (entity.hasComponent<Transform>() && entity.hasComponent<Velocity>() && entity.hasComponent<Collider>()) {
            movableCollidableEntities.push_back(entity);
        }
    }
    return movableCollidableEntities;
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
            addComponentPlayer(entity, componentsJson["Player"]);
        }

        if (componentsJson.contains("Npc")) {
            addComponentNpc(entity, componentsJson["Npc"]);
        }

        if (componentsJson.contains("Intent")) {
            addComponentIntent(entity, componentsJson["Intent"]);
        }

        if (componentsJson.contains("Transform")) {
            addComponentTransform(entity, componentsJson["Transform"]);
        }

        if (componentsJson.contains("Collider")) {
            addComponentCollider(entity, componentsJson["Collider"]);
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
            playerState state = playerStatesMap[componentsJson["State"]["state"]];
            entity.addComponent<State>(state);
        }

        if (componentsJson.contains("Velocity")) {
            json velocityJson = componentsJson["Velocity"];
            entity.addComponent<Velocity>({velocityJson["dx"], velocityJson["dy"], velocityJson["direction"], velocityJson["speed"]});
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
            std::map<Action, bool> actionInput;
            entity.addComponent<Input>({keyStates, justPressed, justReleased, actionInput});
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
            std::map<playerState, AnimationClip> animations;
            for (auto& [animation, animationClips] : animatorJson["Animations"].items()) {
                playerState state = playerStatesMap[animation];
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

            entity.addComponent<Animator>({animations, playerState::IDLE, 0, 0, true});
        }
        
        if (componentsJson.contains("AI")) {
            Transform& transform = entity.getComponent<Transform>();
            std::pair<int, int> patrolStart = {transform.x, transform.y};
            std::string state = componentsJson["AI"]["state"];
            float patrolRange = componentsJson["AI"]["patrolRange"];
            float attackRange = componentsJson["AI"]["attackRange"];
            float pursuitRange = componentsJson["AI"]["pursuitRange"];
            entity.addComponent<AI>({state, patrolStart, patrolRange, attackRange, pursuitRange});
        }
    }

    // Return the new entity
    return entity;
}

void EntityManager::addComponentPlayer(Entity& entity, const json& componentJson) {
    int playerNum = componentJson["playerNum"];
    entity.addComponent<Player>({playerNum});
}

void EntityManager::addComponentNpc(Entity& entity, const json& componentJson) {
    std::string npcType = componentJson["type"];
    entity.addComponent<Npc>({npcType});
}

void EntityManager::addComponentIntent(Entity& entity, const json& componentJson) {
    Action action = Utils::stringToAction(componentJson["action"]);
    Direction direction = Utils::stringToDirection(componentJson["direction"]);
    entity.addComponent<Intent>({action, direction});
}

void EntityManager::addComponentTransform(Entity& entity, const json& componentJson) {
    entity.addComponent<Transform>({componentJson["x"], componentJson["y"], componentJson["scale"]});
}

void EntityManager::addComponentCollider(Entity& entity, const json& componentJson) {
    entity.addComponent<Collider>({componentJson["offsetX"], componentJson["offsetY"], componentJson["width"], componentJson["height"]});
}