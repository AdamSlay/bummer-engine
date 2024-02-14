#define SDL_MAIN_HANDLED

#include <iostream>

#include "GameEngine/GameEngine.h"
#include "Resources/ResourceUtils.h"


int main(int argc, char* argv[]) {
    /**
     * Main function for the game
     */

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    TTF_Font* font = nullptr;
    if (initialize_resource(window, renderer, font) != 0) {
        return 1;
    }

    bool quit = false;
    while (!quit) {
        game_loop(renderer, font, quit);
    }

    close(renderer, window, font);

    std::cout << "Game exited successfully!" << std::endl;
    return 0;
}
