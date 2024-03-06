#include "SoundSystem.h"
#include "../ECS/EventManager.h"


SoundSystem::SoundSystem() {
    EventManager::getInstance().subscribe("jump", [this]() {
        playSound("assets/sounds/zapsplat/zapsplat_cartoon_comic_ruler_twang_wood_short_005_108518.wav");
    });
}

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
    Mix_VolumeChunk(sound, MIX_MAX_VOLUME / 6);
    Mix_PlayChannel(-1, sound, 0);
}

void SoundSystem::stopSound() {
    Mix_HaltChannel(-1);
}
