#include <iostream>

#include "SDL2/SDL_Image.h"

#include "../Config.h"
#include "ResourceUtils.h"

int initialize_resource(SDL_Window*& window, SDL_Renderer*& renderer, TTF_Font*& font) {
    /**
     * Initialize SDL2, SDL2_image, and SDL2_ttf
     */

    // Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_JOYSTICK) < 0) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Initialize window
    window = SDL_CreateWindow("BummerEngine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(window == nullptr) {
        std::cout << "Window could not be created! \nError: " << SDL_GetError() << std::endl;
        return 2;
    }

    // Initialize renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        std::cout << "Renderer could not be initialized! \nError: " << SDL_GetError() << std::endl;
        return 3;
    }

    // Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cout << "SDL_image could not be initialized! \nError: " << IMG_GetError() << std::endl;
        return 4;
    }

    if(TTF_Init() == -1) {
        std::cout << "SDL_ttf could not initialize! \nError: " << TTF_GetError() << std::endl;
        return 5;
    }

    font = TTF_OpenFont(FONT_PATH.c_str(), FONT_SIZE);
    if (font == nullptr) {
        std::cout << "Failed to load font! \nError: " << TTF_GetError() << std::endl;
        return 6;
    }
    return 0;
}


void close(SDL_Renderer*& renderer, SDL_Window*& window, TTF_Font*& font) {
    /**
     * Free renderer and window resources, quit SDL2 and SDL2_image
     */

    // Destroy window/renderer
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    renderer = nullptr;
    window = nullptr;
    font = nullptr;

    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}
