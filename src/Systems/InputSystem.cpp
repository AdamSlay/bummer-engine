#include "InputSystem.h"
#include <SDL2/SDL.h>

void InputSystem::update(EntityManager &entityManager, bool &quit) {
    // Clear justPressed for all entities
    for (Entity &entity : entityManager.getEntities()) {
        if (entity.hasComponent<Input>()) {
            Input &input = entity.getComponent<Input>();
            input.justPressed.clear();
            input.justReleased.clear();
        }
    }

    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            quit = true;
            break;
        }
        else if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
            for (Entity &entity : entityManager.getEntities()) {
                if (entity.hasComponent<Input>()) {
                    Input &input = entity.getComponent<Input>();
                    input.keyStates[e.key.keysym.scancode] = (e.type == SDL_KEYDOWN);
                    if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
                        input.justPressed[e.key.keysym.scancode] = true;
                    }
                    else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
                        input.justReleased[e.key.keysym.scancode] = true;
                    }
                }
            }
        }
    }
}