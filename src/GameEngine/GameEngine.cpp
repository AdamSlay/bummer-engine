#include "GameEngine.h"
#include "../UI/Menu.h"


void game_loop(SDL_Renderer* renderer, TTF_Font* font) {
    /**
     * Main game loop
     *
     * @param renderer: The SDL renderer
     * @param font: The TTF font
     */
    // Render the splash screen on startup
    render_splash_screen(renderer, font);

    SDL_Event e;
    bool quit = false;
    while (!quit) {
        poll_events(e, quit);

        SDL_SetRenderDrawColor(renderer, 104,102,182, 0xFF);
        SDL_RenderClear(renderer);
        // Copy game to renderer here
        SDL_RenderPresent(renderer);
    }
}

void render_splash_screen(SDL_Renderer* renderer, TTF_Font* font) {
    /**
     * Render the splash screen on startup
     *
     * @param renderer: The renderer to render the splash screen to
     * @param font: The font to use for the splash screen
     */
    SDL_SetRenderDrawColor(renderer, 252,226,137, 0xFF);  // bb_yellow
    SDL_RenderClear(renderer);
    splash_screen(renderer, font);
    SDL_RenderPresent(renderer);
    SDL_Delay(5000);
}

void poll_events(SDL_Event& e, bool& quit) {
    /**
     * Polls events and sets the quit flag if the user closes the window
     *
     * @param e: The SDL event
     * @param quit: The quit flag
     */
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
}
