#include "GameEngine.h"
#include "../UI/Menu.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/AnimationSystem.h"

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
    AnimationSystem animationSystem;
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
    Uint32 lastTime = SDL_GetTicks();
    while (!quit) {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;
        if (deltaTime < 1000 / 60) {
            SDL_Delay((1000 / 60) - deltaTime);
        }
        

        Entity& player = entityManager.getPlayer();
        // reset player position if it falls off the screen
        if (player.getComponent<Transform>().y > SCREEN_HEIGHT) {
            player.getComponent<Transform>().y = SCREEN_HEIGHT / 4;
            player.getComponent<Transform>().x = SCREEN_WIDTH / 2;
            player.getComponent<Velocity>().dy = 0;
        }
        player_controller(e, quit, player, movementSystem);

        // Perform game logic updates here
        move_and_collide(entityManager, movementSystem, collisionSystem);
        animationSystem.update(entityManager, deltaTime);

        SDL_SetRenderDrawColor(renderer, 104,102,182, 255);  // bb_purple
        SDL_RenderClear(renderer);
        render_all_colliders(renderer, entityManager);

        // Copy game to renderer here
        renderSystem.render(renderer, entityManager);
        SDL_RenderPresent(renderer);

    }
}

void render_all_colliders(SDL_Renderer* renderer, EntityManager& entityManager) {
    /**
     * Render all colliders for all entities
     *
     * @param renderer: The SDL renderer
     * @param entityManager: The entity manager
     */
    for (Entity& entity : entityManager.getEntities()) {
        if (entity.hasComponent<Collider>()) {
            render_collider(entity, renderer);
        }
    }
}

void render_collider(Entity &entity, SDL_Renderer *renderer) {
    /**
     * Render the collider for an entity
     *
     * @param entity: The entity
     * @param renderer: The SDL renderer
     */
    Transform &pos = entity.getComponent<Transform>();
    Collider &col = entity.getComponent<Collider>();
    Scale& scale = entity.getComponent<Scale>();
    int x = pos.x + (col.offsetX * scale.scale);
    int y = pos.y + (col.offsetY * scale.scale);
    int w = col.width * scale.scale;
    int h = col.height * scale.scale;
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

void player_controller(SDL_Event& e, bool& quit, Entity& player, MovementSystem& movementSystem) {
    /**
     * Polls events and sets the quit flag if the user closes the window
     *
     *
     * @param e: The SDL event
     * @param quit: The quit flag
     */
    while (SDL_PollEvent(&e) != 0) 
    {
        Velocity& vel = player.getComponent<Velocity>();

        const Uint8 *currentKeyStates = SDL_GetKeyboardState(NULL);
        if (currentKeyStates[SDL_SCANCODE_LEFT])
        {
            vel.dx = -5;
        }
        if (currentKeyStates[SDL_SCANCODE_RIGHT])
        {
            vel.dx = 5;
        }

        const Uint8* controllerButtonStates = SDL_GetKeyboardState(NULL);
        if (controllerButtonStates[SDL_CONTROLLER_BUTTON_DPAD_LEFT])
        {
            vel.dx = -5;
        }
        if (controllerButtonStates[SDL_CONTROLLER_BUTTON_DPAD_RIGHT])
        {
            vel.dx = 5;
        }

        if (e.type == SDL_KEYDOWN) 
        {
            switch (e.key.keysym.sym) 
            {
                case SDLK_UP:
                    if (e.key.repeat == 0) 
                    {
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
            case SDL_CONTROLLER_BUTTON_A:
                vel.dy = 0;
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
    Entity& player = entityManager.createPlayer(50, 50, 18, 40);
    Entity& platform2 = entityManager.createPlatform(500, 400, 128, 32);
    Entity& platform3 = entityManager.createPlatform(700, 450, 128, 32);
}
