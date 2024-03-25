#include <iostream>
#include <fstream>
#include <unordered_map>

#include <nlohmann/json.hpp>
#include <SDL2/SDL.h>

#include "InputSystem.h"
#include "../Utils.h"

using json = nlohmann::json;

InputSystem::InputSystem() {
    loadInputMaps();
}

void InputSystem::update(EntityManager &entityManager, bool &quit) {
    /**
     * Update the input system
     *
     * @param entityManager: The entity manager
     * @param quit: The quit flag
     */
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
        if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
            quit = true;
            break;
        }

        for (Entity& entity : entityManager.getEntities()) {
            if (entity.hasComponent<Input>()) {
                Input &input = entity.getComponent<Input>();
                if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
                    handleKeyboardInput(e, input);
                }
                else if (e.type == SDL_CONTROLLERBUTTONDOWN || e.type == SDL_CONTROLLERBUTTONUP) {
                    handleControllerInput(e, input);
                }
                else if (e.type == SDL_JOYAXISMOTION) {
                    handleJoystickInput(e, input);
                }
            }
        }
    }
}

void InputSystem::loadInputMaps() {
    /**
     * Load input maps
     */

    loadScancodeMap("etc/input_maps/scancode_map.json");
    loadControllerMap("etc/input_maps/controller_map.json");
}

void InputSystem::loadScancodeMap(const std::string& filePath) {
    /**
     * Load scancode map from file
     *
     * @param filePath: The path to the scancode map file
     */

    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open scancode map file");
    }
    json j;
    file >> j;
    for (auto& element : j.items()) {
        SDL_Scancode scancode = SDL_GetScancodeFromName(element.key().c_str());
        Action action = Utils::stringToAction(element.value());
        scancodeMap[scancode] = action;
    }
}

void InputSystem::loadControllerMap(const std::string& filePath) {
    /**
     * Load controller map from file
     *
     * @param filePath: The path to the controller map file
     */

    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open controller map file");
    }
    json j;
    file >> j;
    for (auto& element : j.items()) {
        SDL_GameControllerButton button = SDL_GameControllerGetButtonFromString(element.key().c_str());
        Action action = Utils::stringToAction(element.value());
        controllerMap[button] = action;
    }
}

void InputSystem::handleKeyboardInput(SDL_Event& e, Input& input) {
    /**
     * Handle Keyboard input
     *
     * @param e: The SDL event
     * @param input: The input component
     */

    input.keyStates[e.key.keysym.scancode] = (e.type == SDL_KEYDOWN);
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
        input.justPressed[e.key.keysym.scancode] = true;

//        std::string action = Utils::actionToString(scancodeMap[e.key.keysym.scancode]);
//        std::cout << "Scancode: " << e.key.keysym.scancode << " Action: " << action << std::endl;
    }
    else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
        input.justReleased[e.key.keysym.scancode] = true;
    }
}

void InputSystem::handleControllerInput(SDL_Event& e, Input& input) {
    /**
     * Handle Controller input
     *
     * @param e: The SDL event
     * @param input: The input component
     */
    SDL_Scancode scancode = mapControllerButtonToScancode(e.cbutton.button);
    input.keyStates[scancode] = (e.type == SDL_CONTROLLERBUTTONDOWN);
    if (e.type == SDL_CONTROLLERBUTTONDOWN) {
        input.justPressed[scancode] = true;

//        SDL_GameControllerButton button = static_cast<SDL_GameControllerButton>(e.cbutton.button);
//        std::string action = Utils::actionToString(controllerMap[button]);
//        std::cout << "Button: " << button << " Action: " << action << std::endl;
    }
    else if (e.type == SDL_CONTROLLERBUTTONUP) {
        input.justReleased[scancode] = true;
    }

}

void InputSystem::handleJoystickInput(SDL_Event& e, Input& input) {
    /**
     * Handle Joystick input
     *
     * @param e: The SDL event
     * @param input: The input component
     */
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

SDL_Scancode InputSystem::mapControllerButtonToScancode(Uint8 button) {
    /**
     * Map controller buttons to keyboard inputs
     *
     * @param button: The controller button
     */
    switch (button) {
    case SDL_CONTROLLER_BUTTON_A:
        return SDL_SCANCODE_UP; // Map 'A' button to up key
    case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:  // Map 'X' button to Space key
        return SDL_SCANCODE_SPACE;
    case SDL_CONTROLLER_BUTTON_X:
        return SDL_SCANCODE_R;
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