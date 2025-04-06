#include "Utils.h"

#include "ECS/Components.h"

Action Utils::stringToAction(const std::string& action) {
    /**
     * Convert string to Action
     *
     * @param action: The action string
     */
    // TODO: Add some failure state for when the Action is not found like Action::UNKNOWN_ACTION or similar
    std::map<std::string, Action> actionMap = {
            {"WAIT",       Action::WAIT},
            {"JUMP",       Action::JUMP},
            {"DOWN",       Action::DOWN},
            {"MOVE_LEFT",  Action::MOVE_LEFT},
            {"MOVE_RIGHT", Action::MOVE_RIGHT},
            {"ATTACK",     Action::ATTACK},
            {"DASH",       Action::DASH}
    };
    auto it = actionMap.find(action);
    if (it != actionMap.end()) {
        return it->second;
    } else {
        // this is when the action is not found
        // return Action::UNKNOWN_ACTION or similar
        // TODO: add logging here
        return Action::WAIT;
    }
}

Direction Utils::stringToDirection(const std::string& direction) {
    /**
     * Convert string to Direction
     *
     * @param direction: The direction string
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
     * @param action: The action type
     */
    std::map<Action, std::string> actionMap = {
            {Action::WAIT,       "WAIT"},
            {Action::JUMP,       "JUMP"},
            {Action::STOP_JUMP,  "STOP_JUMP"},
            {Action::MOVE_LEFT,  "MOVE_LEFT"},
            {Action::MOVE_RIGHT, "MOVE_RIGHT"},
            {Action::ATTACK,     "ATTACK"},
            {Action::DASH,       "DASH"},
            {Action::DOWN,       "DOWN"}
    };
    return actionMap[action];
}

std::string Utils::playerStateToString(playerState state) {
    /**
     * Convert playerState to string
     *
     * @param state: The player state
     */
    std::map<playerState, std::string> stateMap = {
            {playerState::IDLE,              "IDLE"},
            {playerState::FLYING,              "FLYING"},
            {playerState::GROUNDED,          "GROUNDED"},
            {playerState::RUN,               "RUN"},
            {playerState::JUMP_ASCEND,       "JUMP_ASCEND"},
            {playerState::JUMP_DESCEND,      "JUMP_DESCEND"},
            {playerState::JUMP_APEX,         "JUMP_APEX"},
            {playerState::JUMP_APEX_DESCEND, "JUMP_APEX_DESCEND"},
            {playerState::JUMP_APEX_ASCEND,  "JUMP_APEX_ASCEND"},
            {playerState::BASIC_ATTACK,      "BASIC_ATTACK"},
            {playerState::HIT,               "HIT"},
            {playerState::STUNNED,           "STUNNED"},
            {playerState::DASHING,           "DASHING"}
    };
    return stateMap[state];
}
