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
    render_splash_screen(renderer, font);

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

        // Open controller if added during runtime
        if (SDL_NumJoysticks() > 0) {
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
    Entity& player = entityManager.createEntityFromTemplate("etc/templates/player.json");
    Entity& platform = entityManager.createEntityFromTemplate("etc/templates/ground.json");
    Entity& platform2 = entityManager.createEntityFromTemplate("etc/templates/platform2.json");
    Entity& platform3 = entityManager.createEntityFromTemplate("etc/templates/platform3.json");
    Entity& platform4 = entityManager.createEntityFromTemplate("etc/templates/platform4.json");
    Entity& martian = entityManager.createEntityFromTemplate("etc/templates/alien.json");
}
