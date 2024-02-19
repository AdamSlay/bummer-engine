#define SDL_MAIN_HANDLED

#include <iostream>

#include "Config.h"
#include "GameEngine/GameEngine.h"
#include "Resources/ResourceUtils.h"


int main(int argc, char* argv[]) {
    /**
     * Main function for the game
     *
     * config path is relative to the project root per bin/build-n-run.sh
     */
    loadConfig("etc/run_config.json");

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    TTF_Font* font = nullptr;
    if (initialize_resource(window, renderer, font) != 0) {
        return 1;
    }
    SDL_RaiseWindow(window);  // Raise the window to the top of the stack
    SDL_PumpEvents();  // Pump event queue so the window is visible asap

    game_loop(renderer, font);

    close(renderer, window, font);

    std::cout << "Game exited successfully!" << std::endl;
    return 0;
}
