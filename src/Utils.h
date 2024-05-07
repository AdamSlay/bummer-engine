#pragma once
#ifndef BUMMERENGINE_UTILS_H
#define BUMMERENGINE_UTILS_H

#include <SDL2/SDL.h>
#include "ECS/EntityManager.h"

namespace Utils {
    Action stringToAction(const std::string& action);
    Direction stringToDirection(const std::string& direction);
    std::string playerStateToString(playerStates state);
    std::string actionToString(Action action);

    // TODO: These functions need to be moved to their respective systems
    void publishEvent(std::string eventString, Entity* primaryEntity = nullptr, Entity* secondaryEntity = nullptr);
}
#endif //BUMMERENGINE_UTILS_H
