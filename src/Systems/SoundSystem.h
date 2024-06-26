#pragma once
#ifndef BUMMERENGINE_SOUNDSYSTEM_H
#define BUMMERENGINE_SOUNDSYSTEM_H

#include <SDL2/SDL_mixer.h>
#include "../ECS/EntityManager.h"

class SoundSystem {
public:
    SoundSystem();
    void update(EntityManager& entityManager);
    void playSound(const std::string& soundFile, int volumeDivisor);
    void stopSound();
};

#endif //BUMMERENGINE_SOUNDSYSTEM_H