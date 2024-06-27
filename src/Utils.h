#pragma once
#ifndef BUMMERENGINE_UTILS_H
#define BUMMERENGINE_UTILS_H

#include <SDL2/SDL.h>
#include "ECS/EntityManager.h"

namespace Utils {
    Action stringToAction(const std::string& action);
    Direction stringToDirection(const std::string& direction);
    std::string playerStateToString(playerState state);
    std::string actionToString(Action action);
}
#endif //BUMMERENGINE_UTILS_H
