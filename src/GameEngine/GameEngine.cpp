
#include "GameEngine.h"
#include "../UI/Menu.h"

void game_loop(SDL_Renderer* renderer, TTF_Font* font, bool& quit) {
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_s:
                        return;
                    case SDLK_q:
                        exit(0);
                }
            }
        }
        // Clear the renderer
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
        SDL_RenderClear(renderer);

        // Set up the start screen
        start_menu(renderer, font);

        // Render the screen
        SDL_RenderPresent(renderer);
    }
}
