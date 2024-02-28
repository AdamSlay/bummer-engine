#ifndef BUMMERENGINE_GAMEENGINE_H
#define BUMMERENGINE_GAMEENGINE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../ECS/EntityManager.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/MovementSystem.h"

void game_loop(SDL_Renderer* renderer, TTF_Font* font);
void move_and_collide(EntityManager& entityManager, MovementSystem& movementSystem, CollisionSystem& collisionSystem);
void render_splash_screen(SDL_Renderer* renderer, TTF_Font* font);
void render_collider(Entity& entity, SDL_Renderer* renderer);
void player_controller(SDL_Event& e, bool& quit, Entity& player, MovementSystem& movementSystem);
void sandbox(EntityManager& entityManager);

#endif //BUMMERENGINE_GAMEENGINE_H
