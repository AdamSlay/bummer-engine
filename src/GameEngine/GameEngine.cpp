#include "GameEngine.h"
#include "../UI/Menu.h"
#include "../ECS/Components.h"
#include "../Resources/TextureManager.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/MovementSystem.h"
#include "../Systems/RenderSystem.h"

#include "../Config.h"

void game_loop(SDL_Renderer* renderer, TTF_Font* font) {
    /**
     * Main game loop
     *
     * @param renderer: The SDL renderer
     * @param font: The TTF font
     */
//    render_splash_screen(renderer, font);

    EntityManager entityManager;
    RenderSystem renderSystem;
    MovementSystem movementSystem;
    TextureManager textureManager;
    CollisionSystem collisionSystem;
    entityManager.setTextureManager(&textureManager);
    entityManager.setRenderer(renderer);

    // entity manager testing sandbox, just for testing new features
    sandbox(entityManager);

    SDL_Event e;
    bool quit = false;
    while (!quit) {
        Entity& player = entityManager.getPlayer();
        poll_events(e, quit, player);

        // Perform game logic updates here
        movementSystem.update(entityManager);
        collisionSystem.update(entityManager);

        SDL_SetRenderDrawColor(renderer, 104,102,182, 255);  // bb_purple
        SDL_RenderClear(renderer);

        // Copy game to renderer here
        renderSystem.render(renderer, entityManager);

        SDL_RenderPresent(renderer);
    }
}

void poll_events(SDL_Event& e, bool& quit, Entity& player) {
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
            Velocity& vel = player.getComponent<Velocity>();
            switch (e.key.keysym.sym) {
                case SDLK_UP:
                    vel.dy = -5;
                    break;
                case SDLK_DOWN:
                    vel.dy = 5;
                    break;
                case SDLK_LEFT:
                    vel.dx = -5;
                    break;
                case SDLK_RIGHT:
                    vel.dx = 5;
                    break;
            }
        }
        else if (e.type == SDL_KEYUP)
        {
            Velocity &vel = player.getComponent<Velocity>();
            switch (e.key.keysym.sym)
            {
            case SDLK_UP:
            case SDLK_DOWN:
                vel.dy = 0;
                break;
            case SDLK_LEFT:
            case SDLK_RIGHT:
                vel.dx = 0;
                break;
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
    Entity& player = entityManager.createPlayer(50, 50, 64, 100);
}
