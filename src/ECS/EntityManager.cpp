#include <fstream>
#include <iostream>

#include "EntityManager.h"
#include "../Utils.h"

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

    componentAdders = {
            {"Player", &EntityManager::addComponentPlayer},
            {"Npc", &EntityManager::addComponentNpc},
            {"Intent", &EntityManager::addComponentIntent},
            {"Transform", &EntityManager::addComponentTransform},
            {"Collider", &EntityManager::addComponentCollider},
            {"Sprite", &EntityManager::addComponentSprite},
            {"State", &EntityManager::addComponentState},
            {"Velocity", &EntityManager::addComponentVelocity},
            {"Gravity", &EntityManager::addComponentGravity},
            {"Input", &EntityManager::addComponentInput},
            {"Jumps", &EntityManager::addComponentJumps},
            {"Dash", &EntityManager::addComponentDash},
            {"Health", &EntityManager::addComponentHealth},
            {"AttackMap", &EntityManager::addComponentAttackMap},
            {"Animator", &EntityManager::addComponentAnimator},
            {"AI", &EntityManager::addComponentAI}
    };
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
    ordered_json templateJson = loadTemplateFile(templatePath);

    // Create a new entity
    Entity& entity = createEntity();

    // For each component in the template, add the component to the entity with the specified values
    if (templateJson.contains("components")) {
        ordered_json componentsJson = templateJson["components"];

        for (auto& [componentName, componentJson] : componentsJson.items()) {
            std::cout << componentName << std::endl;
            if (componentAdders.count(componentName) > 0) {
                (this->*componentAdders[componentName])(entity, componentJson);
            }
        }
    }
    return entity;
}

ordered_json EntityManager::loadTemplateFile(const std::string& templatePath) {
    // Open the template file
    std::ifstream file(templatePath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open template file: " + templatePath);
    }

    // Parse the JSON
    ordered_json templateJson;
    file >> templateJson;

    return templateJson;
}

void EntityManager::addComponentPlayer(Entity& entity, const ordered_json& componentJson) {
    int playerNum = componentJson["playerNum"];
    entity.addComponent<Player>({playerNum});
}

void EntityManager::addComponentNpc(Entity& entity, const ordered_json& componentJson) {
    std::string npcType = componentJson["type"];
    entity.addComponent<Npc>({npcType});
}

void EntityManager::addComponentIntent(Entity& entity, const ordered_json& componentJson) {
    Action action = Utils::stringToAction(componentJson["action"]);
    Direction direction = Utils::stringToDirection(componentJson["direction"]);
    entity.addComponent<Intent>({action, direction});
}

void EntityManager::addComponentTransform(Entity& entity, const ordered_json& componentJson) {
    entity.addComponent<Transform>({componentJson["x"], componentJson["y"], componentJson["scale"]});
}

void EntityManager::addComponentCollider(Entity& entity, const ordered_json& componentJson) {
    entity.addComponent<Collider>({componentJson["offsetX"], componentJson["offsetY"], componentJson["width"], componentJson["height"]});
}

void EntityManager::addComponentSprite(Entity& entity, const ordered_json& componentJson) {
    SDL_Texture* texture = textureManager->loadTexture(renderer, componentJson["texturePath"]);
    int x = static_cast<int>(componentJson["srcRect"]["x"]);
    int y = static_cast<int>(componentJson["srcRect"]["y"]);
    int w = static_cast<int>(componentJson["srcRect"]["w"]);
    int h = static_cast<int>(componentJson["srcRect"]["h"]);
    SDL_Rect srcRect = {x, y, w, h};
    entity.addComponent<Sprite>({texture, srcRect});
}

void EntityManager::addComponentVelocity(Entity& entity, const ordered_json& componentJson) {
    entity.addComponent<Velocity>({componentJson["dx"], componentJson["dy"], componentJson["direction"], componentJson["speed"]});
}

void EntityManager::addComponentGravity(Entity& entity, const ordered_json& componentJson) {
    float baseGravity = componentJson["baseGravity"];
    float gravity = componentJson["gravity"];
    float ascendFactor = componentJson["ascendFactor"];
    float descendFactor = componentJson["descendFactor"];
    entity.addComponent<Gravity>({baseGravity, gravity, ascendFactor, descendFactor});
}

void EntityManager::addComponentInput(Entity& entity, const ordered_json& componentJson) {
    std::map<SDL_Scancode, bool> keyStates;
    std::map<SDL_Scancode, bool> justPressed;
    std::map<SDL_Scancode, bool> justReleased;
    std::map<Action, bool> actionInput;
    entity.addComponent<Input>({keyStates, justPressed, justReleased, actionInput});
}

void EntityManager::addComponentAnimator(Entity& entity, const ordered_json& componentJson) {

    // open the animator file
    std::string animatorPath = componentJson["animatorPath"];
    std::ifstream animatorFile(animatorPath);
    if (!animatorFile.is_open()) {
        throw std::runtime_error("Could not open animator file: " + componentJson["animatorPath"].get<std::string>());
    }
    ordered_json animatorJson;
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

void EntityManager::addComponentJumps(Entity& entity, const ordered_json& componentJson) {
    int jumps = componentJson["jumps"];
    int maxJumps = componentJson["maxJumps"];
    int jumpForce = componentJson["jumpVelocity"];
    entity.addComponent<Jumps>({jumps, maxJumps, jumpForce});

}

void EntityManager::addComponentDash(Entity& entity, const ordered_json& componentJson) {
    int speed = componentJson["speed"];
    bool isDashing = false;
    float initCooldown = componentJson["initCooldown"];
    float initDuration = componentJson["initDuration"];
    entity.addComponent<Dash>({speed, isDashing, initCooldown, initDuration});

}

void EntityManager::addComponentHealth(Entity& entity, const ordered_json& componentJson) {
    int maxHealth = componentJson["maxHealth"];
    int currentHealth = componentJson["currentHealth"];
    int invincibilityFrames = componentJson["invincibilityFrames"];
    entity.addComponent<Health>({maxHealth, currentHealth, invincibilityFrames});
}

void EntityManager::addComponentAttackMap(Entity& entity, const ordered_json& componentJson) {
    std::map<std::string, AttackInfo> attacks;

    for (auto& [attackName, attackPath] : componentJson.items()) {
        std::ifstream attackFile(attackPath.get<std::string>());
        if (!attackFile.is_open()) {
            throw std::runtime_error("Could not open attack file: " + attackPath.get<std::string>());
        }
        ordered_json attackInfoJson;
        attackFile >> attackInfoJson;

        int damage = attackInfoJson["damage"];
        bool isActive = false;
       ordered_json hitboxJson = attackInfoJson["hitbox"];
        Hitbox hitbox = {hitboxJson["offsetX"], hitboxJson["offsetY"], hitboxJson["width"], hitboxJson["height"]};
        int knockback = attackInfoJson["knockback"];
        int windupframes = attackInfoJson["windupFrames"];
        int duration = attackInfoJson["duration"];
        attacks.emplace(attackName, AttackInfo{damage, isActive, hitbox, knockback, duration, windupframes});
    }

    entity.addComponent<AttackMap>({attacks});

}

void EntityManager::addComponentState(Entity& entity, const ordered_json& componentJson) {
    playerState state = playerStatesMap[componentJson["state"]];
    entity.addComponent<State>(state);
}

void EntityManager::addComponentAI(Entity& entity, const ordered_json& componentJson) {
    Transform& transform = entity.getComponent<Transform>();
    std::pair<int, int> patrolStart = {transform.x, transform.y};
    std::string state = componentJson["state"];
    float patrolRange = componentJson["patrolRange"];
    float attackRange = componentJson["attackRange"];
    float pursuitRange = componentJson["pursuitRange"];
    entity.addComponent<AI>({state, patrolStart, patrolRange, attackRange, pursuitRange});
}
