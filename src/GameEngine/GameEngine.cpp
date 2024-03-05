#include "GameEngine.h"
#include "../UI/Menu.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/InputSystem.h"
#include "../Systems/PhysicsSystem.h"

#include "../Utils.cpp"
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
    InputSystem inputSystem;
    PhysicsSystem physicsSystem;

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
        // handle frame timing
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;
        if (deltaTime < 1000 / 60) {
            SDL_Delay((1000 / 60) - deltaTime);
        }

        // Perform game logic updates
        inputSystem.update(entityManager, quit);
        physicsSystem.update(entityManager, movementSystem, collisionSystem);
        animationSystem.update(entityManager, deltaTime);

        // render colliders
        SDL_SetRenderDrawColor(renderer, 104,102,182, 255);  // bb_purple
        SDL_RenderClear(renderer);
        Utils::render_all_colliders(entityManager, renderer);

        // Copy game to renderer here
        renderSystem.render(renderer, entityManager);
        SDL_RenderPresent(renderer);

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
