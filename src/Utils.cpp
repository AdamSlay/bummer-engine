#include "Utils.h"

#include "ECS/Components.h"

Action Utils::stringToAction(const std::string& action) {
    /**
     * Convert string to Action
     *
     * @param action: The action string
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
