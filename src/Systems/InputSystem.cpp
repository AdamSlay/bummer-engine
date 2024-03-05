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
        else if (e.type == SDL_CONTROLLERBUTTONDOWN || e.type == SDL_CONTROLLERBUTTONUP) {
            for (Entity &entity : entityManager.getEntities()) {
                if (entity.hasComponent<Input>()) {
                    Input &input = entity.getComponent<Input>();
                    // Map the controller button to an equivalent keyboard key
                    SDL_Scancode scancode = mapControllerButtonToScancode(e.cbutton.button);
                    input.keyStates[scancode] = (e.type == SDL_CONTROLLERBUTTONDOWN);
                    if (e.type == SDL_CONTROLLERBUTTONDOWN) {
                        input.justPressed[scancode] = true;
                    }
                    else if (e.type == SDL_CONTROLLERBUTTONUP) {
                        input.justReleased[scancode] = true;
                    }
                }
            }
        }
    }
}

SDL_Scancode InputSystem::mapControllerButtonToScancode(Uint8 button) {
    switch (button) {
    case SDL_CONTROLLER_BUTTON_A:
        return SDL_SCANCODE_UP; // Map 'A' button to up key
    case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
        return SDL_SCANCODE_LEFT; // Map left D-pad button to left arrow key
    case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
        return SDL_SCANCODE_RIGHT; // Map right D-pad button to right arrow key
    case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
        return SDL_SCANCODE_DOWN;  // Map down D-pad button to down arrow key
    default:
        return SDL_SCANCODE_UNKNOWN;
    }
}