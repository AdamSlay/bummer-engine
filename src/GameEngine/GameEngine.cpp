#include "GameEngine.h"
#include "../UI/Menu.h"


void game_loop(SDL_Renderer* renderer, TTF_Font* font) {
    /**
     * Main game loop
     *
     * @param renderer: The SDL renderer
     * @param font: The TTF font
     */

    SDL_SetRenderDrawColor(renderer, 104,102,182, 0xFF);
    SDL_RenderClear(renderer);
    splash_screen(renderer, font);
    SDL_RenderPresent(renderer);
    SDL_Delay(5000);

    SDL_Event e;
    bool splash = false;
    bool quit = false;
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
        SDL_SetRenderDrawColor(renderer, 104,102,182, 0xFF);
        SDL_RenderClear(renderer);


        // Render the screen
        SDL_RenderPresent(renderer);
    }
}
