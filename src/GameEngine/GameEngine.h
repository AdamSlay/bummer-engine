#pragma once
#ifndef BUMMERENGINE_GAMEENGINE_H
#define BUMMERENGINE_GAMEENGINE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../ECS/EntityManager.h"
#include "../ECS/SceneManager.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/MovementSystem.h"

void game_loop(SDL_Renderer* renderer, TTF_Font* font);
void sandbox(SceneManager& SceneManager);
std::tuple<Uint32, float> incrementTime(Uint32 lastTime, float deltaTime);

#endif //BUMMERENGINE_GAMEENGINE_H
