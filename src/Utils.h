#pragma once
#ifndef BUMMERENGINE_UTILS_H
#define BUMMERENGINE_UTILS_H

#include <SDL2/SDL.h>
#include "ECS/EntityManager.h"

namespace Utils {
    Action stringToAction(std::string action);
    Direction stringToDirection(std::string direction);
    void publishEvent(std::string eventString, Entity* primaryEntity = nullptr, Entity* secondaryEntity = nullptr);
    std::string playerStateToString(playerStates state);
    std::string actionToString(Action action);
    SDL_Rect getColliderRect(const Entity& entity);
    void setTransformPos(Entity& entity, int x, int y);
    void setTransformX(Entity& entity, int x);
    void setTransformY(Entity& entity, int y);
    void render_collider(Entity &entity, SDL_Renderer *renderer);
    void render_all_colliders(EntityManager& entityManager, SDL_Renderer* renderer);
    SDL_Rect getHitboxRect(Hitbox& hitbox, Entity& entity);
    void render_hitboxes(EntityManager& entityManager, SDL_Renderer* renderer);
}
#endif //BUMMERENGINE_UTILS_H
