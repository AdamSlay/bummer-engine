#include "SoundSystem.h"
#include "../ECS/EventManager.h"


SoundSystem::SoundSystem() {
    EventManager::getInstance().subscribe("jump", [this]() {
        playSound("assets/sounds/zapsplat/zapsplat_multimedia_game_sound_retro_arcade_style_jump_001_108838.wav");
    });
    EventManager::getInstance().subscribe("died", [this]() {
        playSound("assets/sounds/zapsplat/Gregor_Quendel_Designed_Mecha_Elements_Impact_01_069.wav");
    });
    EventManager::getInstance().subscribe("spawn", [this]() {
        playSound("assets/sounds/zapsplat/zapsplat_multimedia_game_sound_robotic_buzz_chirp_103482.wav");
    });
    EventManager::getInstance().subscribe("dash", [this]() {
        playSound("assets/sounds/zapsplat/zapsplat_cartoon_whoosh_swipe_fast_grab_dash_006_74747.wav");
    });
    EventManager::getInstance().subscribe("start", [this]() {
        playSound("assets/sounds/volts-theme.wav");
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
