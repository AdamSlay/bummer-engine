#include "Utils.h"

#include "ECS/Components.h"
#include "ECS/EventManager.h"

Action Utils::stringToAction(const std::string& action) {
    /**
     * Convert string to Action
     *
     * @param action: The action
     */
    std::map<std::string, Action> actionMap = {
            {"WAIT",       Action::WAIT},
            {"JUMP",       Action::JUMP},
            {"MOVE_LEFT",  Action::MOVE_LEFT},
            {"MOVE_RIGHT", Action::MOVE_RIGHT},
            {"ATTACK",     Action::ATTACK},
            {"DASH",       Action::DASH}
    };
    auto it = actionMap.find(action);
    if (it != actionMap.end()) {
        return it->second;
    } else {
        return Action::WAIT;
    }
}

Direction Utils::stringToDirection(const std::string& direction) {
    /**
     * Convert string to Direction
     *
     * @param direction: The direction
     */
    std::map<std::string, Direction> directionMap = {
            {"STILL",       Direction::STILL},
            {"LEFT",       Direction::LEFT},
            {"RIGHT",  Direction::RIGHT},
            {"JOY_LEFT",  Direction::JOY_LEFT},
            {"JOY_RIGHT", Direction::JOY_RIGHT},
    };
    auto it = directionMap.find(direction);
    if (it != directionMap.end()) {
        return it->second;
    } else {
        return Direction::STILL;
    }
}

std::string Utils::actionToString(Action action) {
    /**
     * Convert Action to string
     *
     * @param action: The action
     */
    std::map<Action, std::string> actionMap = {
            {Action::WAIT,       "WAIT"},
            {Action::JUMP,       "JUMP"},
            {Action::STOP_JUMP,  "STOP_JUMP"},
            {Action::MOVE_LEFT,  "MOVE_LEFT"},
            {Action::MOVE_RIGHT, "MOVE_RIGHT"},
            {Action::ATTACK,     "ATTACK"},
            {Action::DASH,       "DASH"}
    };
    return actionMap[action];
}

std::string Utils::playerStateToString(playerStates state) {
    /**
     * Convert playerStates to string
     *
     * @param state: The player state
     */
    std::map<playerStates, std::string> stateMap = {
            {playerStates::IDLE,                             "IDLE"},
            {playerStates::GROUNDED,                 "GROUNDED"},
            {playerStates::RUN,                              "RUN"},
            {playerStates::JUMP_ASCEND,            "JUMP_ASCEND"},
            {playerStates::JUMP_DESCEND,          "JUMP_DESCEND"},
            {playerStates::JUMP_APEX,                  "JUMP_APEX"},
            {playerStates::JUMP_APEX_DESCEND,"JUMP_APEX_DESCEND"},
            {playerStates::JUMP_APEX_ASCEND,  "JUMP_APEX_ASCEND"},
            {playerStates::BASIC_ATTACK,             "BASIC_ATTACK"},
            {playerStates::HIT,                                  "HIT"},
            {playerStates::STUNNED,                       "STUNNED"},
            {playerStates::DASHING,                        "DASHING"}
    };
    return stateMap[state];
}

void Utils::publishEvent(std::string eventString, Entity* primaryEntity, Entity* secondaryEntity) {
    int entityId = primaryEntity->getID();
    EventManager::getInstance().publish(eventString, {entityId, primaryEntity, secondaryEntity});
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
    SDL_Rect playerCollider = entity.getColliderRect();

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
