#include "GameEngine.h"
#include "../UI/Menu.h"
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
    MovementSystem movementSystem;
    TextureManager textureManager;
    CollisionSystem collisionSystem;
    entityManager.setTextureManager(&textureManager);
    entityManager.setRenderer(renderer);
    SDL_GameController* controller = nullptr;
    if (SDL_NumJoysticks() > 0) {
        controller = SDL_GameControllerOpen(0);
    }

    // entity manager testing sandbox, just for testing new features
    sandbox(entityManager);

    SDL_Event e;
    bool quit = false;
    while (!quit) {
        Entity& player = entityManager.getPlayer();
        poll_events(e, quit, player, movementSystem);

        // Perform game logic updates here
        move_and_collide(entityManager, movementSystem, collisionSystem);

        SDL_SetRenderDrawColor(renderer, 104,102,182, 255);  // bb_purple
        SDL_RenderClear(renderer);

        render_collider(player, renderer);
        render_collider(entityManager.getEntities()[0], renderer);
        // Copy game to renderer here
        renderSystem.render(renderer, entityManager);

        SDL_RenderPresent(renderer);
    }
}

void render_collider(Entity& entity, SDL_Renderer* renderer) {
    /**
     * Render the collider of an entity
     *
     * @param entity: The entity
     */
    int x = entity.getComponent<Position>().x;
    int y = entity.getComponent<Position>().y;
    int w = entity.getComponent<Collider>().width;
    int h = entity.getComponent<Collider>().height;
    SDL_Rect collider = {x, y, w, h};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &collider);

}

void move_and_collide(EntityManager& entityManager, MovementSystem& movementSystem, CollisionSystem& collisionSystem) {
    /**
     * Move entities and check for collisions
     *
     * @param entityManager: The entity manager
     */
    movementSystem.moveX(entityManager);
    collisionSystem.updateX(entityManager);
    movementSystem.moveY(entityManager);
    collisionSystem.updateY(entityManager);
}

void poll_events(SDL_Event& e, bool& quit, Entity& player, MovementSystem& movementSystem) {
    /**
     * Polls events and sets the quit flag if the user closes the window
     *
     * @param e: The SDL event
     * @param quit: The quit flag
     */
    while (SDL_PollEvent(&e) != 0) {
        Velocity& vel = player.getComponent<Velocity>();
        
        if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_UP:
                    if (e.key.repeat == 0) {
                        movementSystem.jump(player);
                    }
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
        else if (e.type == SDL_CONTROLLERBUTTONDOWN)
        {
            switch (e.cbutton.button)
            {
            case SDL_CONTROLLER_BUTTON_A:
                movementSystem.jump(player);
                break;
            case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
                vel.dx = 5;
                break;
            case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
                vel.dx = -5;
                break;
            }
        }
        else if (e.type == SDL_CONTROLLERBUTTONUP)
        {
            Velocity &vel = player.getComponent<Velocity>();
            switch (e.cbutton.button)
            {
            case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
            case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
                vel.dx = 0;
                break;
            }
        }
        else if (e.type == SDL_QUIT) {
            quit = true;
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
    int groundY = SCREEN_HEIGHT - 200;
    int groundW = SCREEN_WIDTH - 100;
    int groundH = 50;
    Entity& platform = entityManager.createPlatform(groundX, groundY, groundW, groundH);
    Entity& player = entityManager.createPlayer(50, 50, 64, 100);
}
