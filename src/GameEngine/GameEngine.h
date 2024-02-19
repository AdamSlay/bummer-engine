#ifndef BUMMERENGINE_GAMEENGINE_H
#define BUMMERENGINE_GAMEENGINE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

void game_loop(SDL_Renderer* renderer, TTF_Font* font);
void render_splash_screen(SDL_Renderer* renderer, TTF_Font* font);
void poll_events(SDL_Event& e, bool& quit);

#endif //BUMMERENGINE_GAMEENGINE_H
