#pragma once
#ifndef BUMMERENGINE_RESOURCEUTILS_H
#define BUMMERENGINE_RESOURCEUTILS_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

int initialize_resource(SDL_Window*& window, SDL_Renderer*& renderer, TTF_Font*& font);
void close(SDL_Renderer*& renderer, SDL_Window*& window, TTF_Font*& font);

#endif //BUMMERENGINE_RESOURCEUTILS_H
