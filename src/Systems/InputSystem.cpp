#include <iostream>
#include <fstream>
#include <unordered_map>

#include <nlohmann/json.hpp>
#include <SDL2/SDL.h>

#include "InputSystem.h"
#include "../Utils.h"
#include "../Config.h"

using json = nlohmann::json;

InputSystem::InputSystem() {
    loadInputMaps();
}

void InputSystem::loadInputMaps() {
    /**
     * Load input maps from path defined in run config
     */
    loadScancodeMap(SCANCODE_MAP_PATH);
    loadControllerMap(CONTROLLER_MAP_PATH);
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

void InputSystem::update(EntityManager &entityManager, bool &start_menu) {
    /**
     * Update the input system
     *
     * @param entityManager: The entity manager
     * @param quit: The quit flag
     */

    clearPreviousInputs(entityManager);

    SDL_Event e;
    Entity player = entityManager.getPlayer();
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
            start_menu = true;
            break;
        }

        auto& input = player.getComponent<Input>();
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
    updateIntent(player);
}

void InputSystem::clearPreviousInputs(EntityManager& entityManager) {
    /**
     * Clear all previous inputs from last frame
     */
    for (Entity &entity : entityManager.getEntities()) {
        if (entity.hasComponent<Input>()) {
            auto& input = entity.getComponent<Input>();
            input.justPressed.clear();
            input.justReleased.clear();
            input.actionInput.clear();
        }
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
        auto it = scancodeMap.find(e.key.keysym.scancode);
        if (it != scancodeMap.end()) {
            input.justPressed[e.key.keysym.scancode] = true;
            input.actionInput[it->second] = true;
        }
    }
    else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
        auto it = scancodeMap.find(e.key.keysym.scancode);
        if (it != scancodeMap.end()) {
            input.justReleased[e.key.keysym.scancode] = true;
            input.actionInput[it->second] = false;
        }
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
    auto button = static_cast<SDL_GameControllerButton>(e.cbutton.button);
    input.keyStates[scancode] = (e.type == SDL_CONTROLLERBUTTONDOWN);
    if (e.type == SDL_CONTROLLERBUTTONDOWN) {
        auto it = controllerMap.find(button);
        if (it != controllerMap.end()) {
            input.justPressed[scancode] = true;
            input.actionInput[it->second] = true;
        }
    }
    else if (e.type == SDL_CONTROLLERBUTTONUP) {
        auto it = controllerMap.find(button);
        if (it != controllerMap.end()) {
            input.justReleased[scancode] = true;
            input.actionInput[it->second] = false;
        }
    }

}

void InputSystem::handleJoystickInput(SDL_Event& e, Input& input) const {
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
            input.keyStates[SDL_SCANCODE_UP] = false;
            input.keyStates[SDL_SCANCODE_DOWN] = true;
        } else {
            input.justReleased[SDL_SCANCODE_UP] = true;
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
    case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:  // Map 'RB' button to Space key
        return SDL_SCANCODE_SPACE;
    case SDL_CONTROLLER_BUTTON_X:
        return SDL_SCANCODE_R;
    case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
        return SDL_SCANCODE_LEFT; // Map left D-pad button to left arrow key
    case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
        return SDL_SCANCODE_RIGHT; // Map right D-pad button to right arrow key
    case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
        return SDL_SCANCODE_DOWN;  // Map down D-pad button to down arrow key
    case SDL_CONTROLLER_BUTTON_DPAD_UP:
        return SDL_SCANCODE_UP;  // Map down D-pad button to down arrow key
    default:
        return SDL_SCANCODE_UNKNOWN;
    }
}

void InputSystem::updateIntent(Entity& player) {
    // clear previous intent
    player.resetIntent(true);

    auto& input = player.getComponent<Input>();
    auto& intent = player.getComponent<Intent>();
    if (input.actionInput[Action::JUMP]) {
        intent.action = Action::JUMP;
    }
    if (input.keyStates[SDL_SCANCODE_UP] == true) {
        intent.action = Action::JUMP;
    }
    if (input.keyStates[SDL_SCANCODE_DOWN] == true) {
        intent.action = Action::DOWN;
    }
    if (input.actionInput[Action::DOWN]) {
        intent.action = Action::DOWN;
    }
    if (input.justReleased[SDL_SCANCODE_UP]) {
        intent.action = Action::STOP_JUMP;
    }
    if (input.justReleased[SDL_SCANCODE_DOWN]) {
        intent.action = Action::STOP_DOWN;
    }
    if (input.actionInput[Action::DASH]) {
        intent.action = Action::DASH;
    }
    if (input.actionInput[Action::ATTACK]) {
        intent.action = Action::ATTACK;
    }
    if (input.actionInput[Action::MOVE_LEFT]) {
        intent.direction = Direction::LEFT;
    }
    if (input.actionInput[Action::MOVE_RIGHT]) {
        intent.direction = Direction::RIGHT;
    }
    if (input.keyStates[SDL_SCANCODE_LEFT]) {
        intent.direction = Direction::LEFT;
    }
    else if (input.keyStates[SDL_SCANCODE_RIGHT]) {
        intent.direction = Direction::RIGHT;
    }
    else {
        intent.direction = Direction::STILL;
    }
}