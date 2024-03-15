#include "GameEngine.h"
#include "../UI/Menu.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/InputSystem.h"
#include "../Systems/PhysicsSystem.h"
#include "../Systems/SoundSystem.h"

#include "../Utils.cpp"
#include "../Config.h"
#include "../ECS/EventManager.h"

void game_loop(SDL_Renderer* renderer, TTF_Font* font) {
    /**
     * Main game loop
     *
     * @param renderer: The SDL renderer
     * @param font: The TTF font
     */
//    render_splash_screen(renderer, font);

    TextureManager textureManager;
    EntityManager entityManager(&textureManager, renderer);
    
    AnimationSystem animationSystem;
    CollisionSystem collisionSystem;
    InputSystem inputSystem;
    MovementSystem movementSystem;
    PhysicsSystem physicsSystem;
    RenderSystem renderSystem;
    SoundSystem soundSystem;

    // Setup controller
    SDL_GameController* controller = nullptr;

    // entity manager testing sandbox, just for testing new features
    sandbox(entityManager);
    EventManager::getInstance().publish("start");

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
        if (SDL_NumJoysticks() > 0) {
            // Open controller if added during runtime
            controller = SDL_GameControllerOpen(0);
        }

        // Perform game logic updates
        inputSystem.update(entityManager, quit);
        physicsSystem.update(entityManager, movementSystem, collisionSystem, deltaTime);
        animationSystem.update(entityManager, deltaTime);
//        soundSystem.update(entityManager);

        // render colliders
        SDL_SetRenderDrawColor(renderer, 104, 102, 184, 255);  // bb_purple
        SDL_RenderClear(renderer);
//        Utils::render_all_colliders(entityManager, renderer);

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
    int groundX = 100;
    int groundY = SCREEN_HEIGHT - 150;
    Entity& platform = entityManager.createPlatform(groundX, groundY, 256, 64, 4.2);
    Entity& player = entityManager.createPlayer(620, 50, 18, 40);
    Entity& platform2 = entityManager.createEntityFromTemplate("etc/templates/platform.json");
    Entity& platform3 = entityManager.createPlatform(832, 400, 256, 32, 1);
    Entity& platform4 = entityManager.createPlatform(512, 250, 256, 32, 1);
    Entity& martian = entityManager.createMartian(400, 50, 18, 60);
}
