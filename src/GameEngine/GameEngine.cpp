#include "GameEngine.h"
#include "../UI/Menu.h"
#include "../ECS/Components.h"
#include "../Systems/RenderSystem.h"

#include "../Config.h"

void game_loop(SDL_Renderer* renderer, TTF_Font* font) {
    /**
     * Main game loop
     *
     * @param renderer: The SDL renderer
     * @param font: The TTF font
     */
    render_splash_screen(renderer, font);

    EntityManager entityManager;
    RenderSystem renderSystem;

    // entity manager testing sandbox, just for testing new features
    sandbox(entityManager);

    SDL_Event e;
    bool quit = false;
    while (!quit) {
        poll_events(e, quit);

        // Perform game logic updates here

        SDL_SetRenderDrawColor(renderer, 104,102,182, 0xFF);  // bb_purple
        SDL_RenderClear(renderer);

        // Copy game to renderer here
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

void sandbox(EntityManager& entityManager) {
    /**
     * Sandbox function for testing new features
     *
     * Create level, entities/components, etc.
     *
     * Called from game_loop(), before the main game loop starts
     *
     * @param entityManager: The entity manager
     */
    int groundX = 50;
    int groundY = SCREEN_HEIGHT - 50;
    int groundW = SCREEN_WIDTH - 100;
    int groundH = 50;
    Entity& platform = entityManager.createPlatform(groundX, groundY, groundW, groundH);
}
