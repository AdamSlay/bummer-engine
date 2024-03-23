#ifndef BUMMERENGINE_INPUTSYSTEM_H
#define BUMMERENGINE_INPUTSYSTEM_H

#include "../ECS/EntityManager.h"

class InputSystem {
public:
    void update(EntityManager& entityManager, bool& quit);
    SDL_Scancode mapControllerButtonToScancode(Uint8 button);
private:
    float deadZone = 0.3f;
    void handleKeyboardInput(SDL_Event& e, Input& input);
    void handleControllerInput(SDL_Event& e, Input& input);
    void handleJoystickInput(SDL_Event& e, Input& input);
};

#endif //BUMMERENGINE_INPUTSYSTEM_H