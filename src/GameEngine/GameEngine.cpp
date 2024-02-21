#include "GameEngine.h"
#include "../UI/Menu.h"
#include "../ECS/EntityManager.h"
#include "../ECS/Components.h"
#include "../Systems/RenderSystem.h"


void game_loop(SDL_Renderer* renderer, TTF_Font* font) {
    /**
     * Main game loop
     *
     * @param renderer: The SDL renderer
     * @param font: The TTF font
     */
//    render_splash_screen(renderer, font);

    // entity manager testing
    EntityManager entityManager;
    RenderSystem renderSystem;
    Entity& platform = entityManager.createPlatform(400, 400, 200, 50);
    Entity& platform2 = entityManager.createPlatform(100, 100, 200, 50);

    // end testing

    SDL_Event e;
    bool quit = false;
    while (!quit) {
        poll_events(e, quit);

        // Perform game logic updates here

        SDL_SetRenderDrawColor(renderer, 104,102,182, 0xFF);
        SDL_RenderClear(renderer);

        // Copy game to renderer here
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        renderSystem.render(renderer, entityManager);

        SDL_RenderPresent(renderer);
    }
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
