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
void move_and_collide(EntityManager& entityManager, MovementSystem& movementSystem, CollisionSystem& collisionSystem);
void render_splash_screen(SDL_Renderer* renderer, TTF_Font* font);
void render_all_colliders(SDL_Renderer* renderer, EntityManager& entityManager);
void render_collider(Entity& entity, SDL_Renderer* renderer);
void player_controller(SDL_Event& e, bool& quit, Entity& player, MovementSystem& movementSystem);
void sandbox(SceneManager& SceneManager);
std::tuple<Uint32, float> incrementTime(Uint32 lastTime, float deltaTime);

#endif //BUMMERENGINE_GAMEENGINE_H
