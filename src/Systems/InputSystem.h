#pragma once
#ifndef BUMMERENGINE_INPUTSYSTEM_H
#define BUMMERENGINE_INPUTSYSTEM_H

#include "../ECS/EntityManager.h"

class InputSystem {
public:
    InputSystem();
    void update(EntityManager& entityManager, bool& quit);

private:
    std::unordered_map<SDL_Scancode, Action> scancodeMap;
    std::unordered_map<SDL_GameControllerButton, Action> controllerMap;
    float deadZone = 0.3f;

    void loadInputMaps();
    void loadControllerMap(const std::string& filepath);
    void loadScancodeMap(const std::string& filepath);
    void clearPreviousInputs(EntityManager& entityManager);
    void handleKeyboardInput(SDL_Event& e, Input& input);
    void handleControllerInput(SDL_Event& e, Input& input);
    void handleJoystickInput(SDL_Event& e, Input& input) const;
    void updateIntent(Entity& player);
    static SDL_Scancode mapControllerButtonToScancode(Uint8 button);
};

#endif //BUMMERENGINE_INPUTSYSTEM_H