#include "SoundSystem.h"


void SoundSystem::update(EntityManager& entityManager) {
    for (Entity& entity : entityManager.getEntities()) {
        if (entity.hasComponent<Sound>()) {
            Sound& sound = entity.getComponent<Sound>();
            playSound(sound.soundFile);
        }
    }
}

void SoundSystem::playSound(const std::string& soundFile) {
    Mix_Chunk* sound = Mix_LoadWAV(soundFile.c_str());
    if (sound == nullptr) {
        // Handle error
    }
    Mix_PlayChannel(-1, sound, 0);
}

void SoundSystem::stopSound() {
    Mix_HaltChannel(-1);
}
