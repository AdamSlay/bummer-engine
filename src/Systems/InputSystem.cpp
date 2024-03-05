#include "InputSystem.h"
#include <SDL2/SDL.h>

void InputSystem::update(EntityManager& entityManager, bool& quit) {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            quit = true;
        }
        else if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
            for (Entity& entity : entityManager.getEntities()) {
                if (entity.hasComponent<Input>()) {
                    Input& input = entity.getComponent<Input>();
                    input.keyStates[e.key.keysym.scancode] = (e.type == SDL_KEYDOWN);
                }
            }
        }
    }
}