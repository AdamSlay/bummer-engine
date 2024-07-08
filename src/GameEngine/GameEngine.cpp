#include "GameEngine.h"
#include "../UI/SplashScreen.h"
#include "../UI/Menu.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/InputSystem.h"
#include "../Systems/PhysicsSystem.h"
#include "../Systems/SoundSystem.h"
#include "../Systems/AttackSystem.h"
#include "../Systems/AISystem.h"
#include "../Systems/CooldownSystem.h"

#include "../Config.h"
#include "../ECS/StateMachine.h"
#include "../ECS/EventManager.h"
#include "../Utils.cpp"

void game_loop(SDL_Renderer* renderer, TTF_Font* font) {
    /**
     * Main game loop
     *
     * @param renderer: The SDL renderer
     * @param font: The TTF font
     */
    render_splash_screen(renderer, font);

    Menu menu(renderer, font);

    TextureManager textureManager;
    EntityManager entityManager(&textureManager, renderer);
    SceneManager sceneManager(entityManager);
    StateMachine stateMachine(entityManager);
    
    AnimationSystem animationSystem;
    CollisionSystem collisionSystem;
    CooldownSystem cooldownSystem;
    InputSystem inputSystem;
    MovementSystem movementSystem;
    PhysicsSystem physicsSystem;
    RenderSystem renderSystem;
//    SoundSystem soundSystem;
    AttackSystem attackSystem;
    AISystem aiSystem;

    // Setup controller
    SDL_GameController* controller = nullptr;

    // entity manager testing sandbox, just for testing new features
    sandbox(sceneManager);
    EventManager::getInstance().publish("start", {});

    bool quit = false;
    bool startMenu = true;
    Uint32 lastTime = SDL_GetTicks();
    float deltaTime = 0.0f;
    SDL_RenderSetLogicalSize(renderer, VIRTUAL_WIDTH, VIRTUAL_HEIGHT);

    while (!quit) {
        // handle frame timing
        std::tie(lastTime, deltaTime) = incrementTime(lastTime, deltaTime);

        // Open controller if added during runtime
        if (SDL_NumJoysticks() > 0) {
            controller = SDL_GameControllerOpen(0);
        }

        // Show Start Menu if necessary
        if (startMenu) {
            menu.update(startMenu, quit);
        }

        // Perform game logic updates
        inputSystem.update(entityManager, startMenu);
        aiSystem.update(entityManager);
        cooldownSystem.update(entityManager, deltaTime);
        attackSystem.update(entityManager);
        physicsSystem.update(sceneManager, entityManager, movementSystem, collisionSystem, deltaTime);
        animationSystem.update(entityManager);

        // Render updates
        SDL_SetRenderDrawColor(renderer, 124, 200, 255, 255);  // sky blue
        SDL_RenderClear(renderer);
        renderSystem.render(renderer, entityManager, font);
        SDL_RenderPresent(renderer);

    }
}


void sandbox(SceneManager& sceneManager) {
    /**
     * Sandbox function for testing new features
     *
     * Create level, entities/components, etc.
     *
     * Called from game_loop(), before the main game loop starts
     *
     * @param entityManager: The entity manager
     */
     sceneManager.nextScene();
}

std::tuple<Uint32, float> incrementTime(Uint32 lastTime, float deltaTime) {
    /**
     * Increment time between frames
     *
     * @param lastTime: The time of the last frame
     * @param deltaTime: The time between frames
     */
    Uint32 currentTime = SDL_GetTicks();
    deltaTime = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;
    if (deltaTime < 1000 / 60) {
        SDL_Delay((1000 / 60) - deltaTime);
    }
    return {lastTime, deltaTime};
}
