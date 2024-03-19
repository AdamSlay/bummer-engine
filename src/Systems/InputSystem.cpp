#include "InputSystem.h"
#include <SDL2/SDL.h>
#include <iostream>

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
        else if (e.type == SDL_MOUSEBUTTONDOWN) {
            if (e.button.button == SDL_BUTTON_LEFT) {
                for (Entity &entity : entityManager.getEntities()) {
                    if (entity.hasComponent<Input>() && entity.hasComponent<AttackMap>()) {
                        entity.getComponent<AttackMap>().attacks["basic"].isActive = true;
                        std::cout << "Attack!" << std::endl;
                    }
                }
            }
        }


        else if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
                quit = true;
                break;
            }
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
        else if (e.type == SDL_JOYAXISMOTION) {
            for (Entity &entity : entityManager.getEntities()) {
                if (entity.hasComponent<Input>()) {
                    Input &input = entity.getComponent<Input>();
                    if (e.jaxis.axis == 0) { // X axis
                        input.joystickDirection.first = e.jaxis.value / 32767.0f;
                        if (input.joystickDirection.first < -deadZone) {
                            input.keyStates[SDL_SCANCODE_LEFT] = true;
                            input.keyStates[SDL_SCANCODE_RIGHT] = false;
                        } else if (input.joystickDirection.first > deadZone) {
                            input.keyStates[SDL_SCANCODE_RIGHT] = true;
                            input.keyStates[SDL_SCANCODE_LEFT] = false;
                        } else {
                            input.keyStates[SDL_SCANCODE_RIGHT] = false;
                            input.keyStates[SDL_SCANCODE_LEFT] = false;
                        }
                    }
                    if (e.jaxis.axis == 1) { // Y axis
                        input.joystickDirection.second = e.jaxis.value / 32767.0f;
                        if (input.joystickDirection.second < -deadZone) {
                            input.keyStates[SDL_SCANCODE_UP] = true;
                            input.keyStates[SDL_SCANCODE_DOWN] = false;
                        } else if (input.joystickDirection.second > deadZone) {
                            input.keyStates[SDL_SCANCODE_DOWN] = true;
                            input.keyStates[SDL_SCANCODE_UP] = false;
                        } else {
                            input.keyStates[SDL_SCANCODE_DOWN] = false;
                            input.keyStates[SDL_SCANCODE_UP] = false;
                        }
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
    case SDL_CONTROLLER_BUTTON_X:  // Map 'X' button to Space key
        return SDL_SCANCODE_SPACE;
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