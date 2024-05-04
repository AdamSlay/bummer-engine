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

    // TODO: These tests need to be moved to their respective test suites
    void publishEvent(std::string eventString, Entity* primaryEntity = nullptr, Entity* secondaryEntity = nullptr);
    SDL_Rect getHitboxRect(Hitbox& hitbox, Entity& entity);
    void render_collider(Entity &entity, SDL_Renderer *renderer);
    void render_all_colliders(EntityManager& entityManager, SDL_Renderer* renderer);
    void render_hitboxes(EntityManager& entityManager, SDL_Renderer* renderer);
}
#endif //BUMMERENGINE_UTILS_H
