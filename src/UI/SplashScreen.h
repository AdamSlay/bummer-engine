#pragma once
#ifndef BUMMERENGINE_SPLASHSCREEN_H
#define BUMMERENGINE_SPLASHSCREEN_H

#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

void render_splash_screen(SDL_Renderer* renderer, TTF_Font* font);
void splash_screen(SDL_Renderer* renderer, TTF_Font* font);
void render_text(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, SDL_Color color, int x, int y, int wrapLength = 0);
#endif //BUMMERENGINE_SPLASHSCREEN_H
