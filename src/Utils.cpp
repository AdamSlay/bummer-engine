#include "Utils.h"

#include "ECS/Components.h"
#include "ECS/EventManager.h"

Action Utils::stringToAction(std::string action) {
    std::map<std::string, Action> actionMap = {
            {"WAIT",       Action::WAIT},
            {"JUMP",       Action::JUMP},
            {"MOVE_LEFT",  Action::MOVE_LEFT},
            {"MOVE_RIGHT", Action::MOVE_RIGHT},
            {"ATTACK",     Action::ATTACK},
            {"DASH",       Action::DASH}
    };
    return actionMap[action];
}

std::string Utils::playerStateToString(playerStates state) {
    switch(state) {
        case playerStates::IDLE: return "IDLE";
        case playerStates::GROUNDED: return "GROUNDED";
        case playerStates::RUN: return "RUN";
        case playerStates::JUMP_ASCEND: return "JUMP_ASCEND";
        case playerStates::JUMP_DESCEND: return "JUMP_DESCEND";
        case playerStates::JUMP_APEX: return "JUMP_APEX";
        case playerStates::JUMP_APEX_DESCEND: return "JUMP_APEX_DESCEND";
        case playerStates::JUMP_APEX_ASCEND: return "JUMP_APEX_ASCEND";
        case playerStates::BASIC_ATTACK: return "BASIC_ATTACK";
        case playerStates::HIT: return "HIT";
        case playerStates::STUNNED: return "STUNNED";
        default: return "UNKNOWN";
    }
}

void Utils::publishEvent(std::string eventString, Entity* primaryEntity, Entity* secondaryEntity) {
    int entityId = primaryEntity->getID();
    EventManager::getInstance().publish(eventString, {entityId, primaryEntity, secondaryEntity});
}


SDL_Rect Utils::getColliderRect(const Entity& entity) {
    /**
     * Get the position of the collider
     *
     * @param entity: The entity
     * @return: A pair of integers representing the x and y position of the collider
     */

    const Transform& transform = entity.getComponent<Transform>();
    const Collider& collider = entity.getComponent<Collider>();
    int x = transform.x + (collider.offsetX * transform.scale);
    int y = transform.y + (collider.offsetY * transform.scale);
    int w = collider.width * transform.scale;
    int h = collider.height * transform.scale;
    return SDL_Rect {x, y, w, h};
}

void Utils::setTransformPos(Entity& entity, int x, int y) {
    /**
     * Set the position of the transform
     *
     * @param entity: The entity
     * @param x: The x position
     * @param y: The y position
     */

    Transform& transform = entity.getComponent<Transform>();
    const Collider& collider = entity.getComponent<Collider>();
    transform.x = x - (collider.offsetX * transform.scale);
    transform.y = y - (collider.offsetY * transform.scale);
}

void Utils::setTransformX(Entity& entity, int x) {
    /**
     * Set the x position of the transform
     *
     * @param entity: The entity
     * @param x: The x position
     */

    Transform& transform = entity.getComponent<Transform>();
    const Collider& collider = entity.getComponent<Collider>();
    transform.x = x - (collider.offsetX * transform.scale);
}

void Utils::setTransformY(Entity& entity, int y) {
    /**
     * Set the y position of the transform
     *
     * @param entity: The entity
     * @param y: The y position
     */

    Transform& transform = entity.getComponent<Transform>();
    const Collider& collider = entity.getComponent<Collider>();
    transform.y = y - (collider.offsetY * transform.scale);
}

void Utils::render_collider(Entity &entity, SDL_Renderer *renderer) {
    /**
     * Render the collider for an entity
     *
     * @param entity: The entity
     * @param renderer: The SDL renderer
     */

    Transform &transform = entity.getComponent<Transform>();
    Collider &col = entity.getComponent<Collider>();
    int x = transform.x + (col.offsetX * transform.scale);
    int y = transform.y + (col.offsetY * transform.scale);
    int w = col.width * transform.scale;
    int h = col.height * transform.scale;
    SDL_Rect collider = {x, y, w, h};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &collider);
}

void Utils::render_all_colliders(EntityManager& entityManager, SDL_Renderer* renderer) {
    /**
     * Render all colliders for all entities
     *
     * @param renderer: The SDL renderer
     * @param entityManager: The entity manager
     */

    for (Entity& entity : entityManager.getEntities()) {
        if (entity.hasComponent<Collider>()) {
            Utils::render_collider(entity, renderer);
        }
    }
}

SDL_Rect Utils::getHitboxRect(Hitbox& hitbox, Entity& entity) {
    /**
     * Get the position of the hitbox relative to entity collider
     *
     * @param hitbox: The hitbox
     * @param entity: The entity
     * @return: A pair of integers representing the x and y position of the hitbox
     */

    SDL_Rect hitboxRect;
    SDL_Rect playerCollider = getColliderRect(entity);

    int direction = entity.getComponent<Velocity>().direction;
    if (direction == 1) { // player is facing right
        hitboxRect.x = playerCollider.x + hitbox.offsetX + playerCollider.w;
    } else { // player is facing left
        hitboxRect.x = playerCollider.x - hitbox.offsetX - hitbox.width;
    }
    hitboxRect.y = playerCollider.y + hitbox.offsetY;
    hitboxRect.w = hitbox.width;
    hitboxRect.h = hitbox.height;

    return hitboxRect;
}

void Utils::render_hitboxes(EntityManager &entityManager, SDL_Renderer *renderer) {
    /**
     * Render all active hitboxes for all entities
     *
     * @param renderer: The SDL renderer
     * @param entityManager: The entity manager
     */

    for (Entity& entity : entityManager.getEntities()) {
        if (entity.hasComponent<AttackMap>()) {
            for (auto& [name, attackInfo] : entity.getComponent<AttackMap>().attacks) {
                if (attackInfo.isActive) {
                    Hitbox& hitbox = attackInfo.hitbox;
                    SDL_Rect hitboxRect = getHitboxRect(hitbox, entity);
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                    SDL_RenderDrawRect(renderer, &hitboxRect);
                }
            }
        }
    }
}