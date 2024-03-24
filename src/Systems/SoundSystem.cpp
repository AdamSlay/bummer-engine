#include "SoundSystem.h"
# include <iostream>
#include "../ECS/EventManager.h"


SoundSystem::SoundSystem() {
    EventManager::getInstance().subscribe("jump", [this](EventData data) {
        playSound("assets/sounds/foly/bb_char/jump_6.wav", 2);
    });
    EventManager::getInstance().subscribe("died", [this](EventData data) {
        playSound("assets/sounds/zapsplat/zapsplat_impacts_body_hit_thud_stab_squelch_of_blood_90708.wav", 2);
    });
    EventManager::getInstance().subscribe("spawn", [this](EventData data) {
        playSound("assets/sounds/zapsplat/zapsplat_sound_design_rewind_reversed_vibration_001_19653.wav", 2);
    });
    EventManager::getInstance().subscribe("dash", [this](EventData data) {
        playSound("assets/sounds/zapsplat/zapsplat_cartoon_whoosh_swipe_fast_grab_dash_006_74747.wav", 4);
    });
    EventManager::getInstance().subscribe("start", [this](EventData data) {
        playSound("assets/sounds/foly/background_environ/waves_birds_long_loop.wav", 4);
        playSound("assets/sounds/music/Sitar_Meditations.wav", 6);
    });
    EventManager::getInstance().subscribe("landed", [this](EventData data) {
        playSound("assets/sounds/foly/bb_char/sand_walk_1.wav", 6);
    });
    EventManager::getInstance().subscribe("basicAttack", [this](EventData data) {
        playSound("assets/sounds/foly/bb_char/attack_6.wav", 2);
    });
    EventManager::getInstance().subscribe("enemyHit", [this](EventData data) {
        playSound("assets/sounds/foly/alien_sounds/take_hit_1.wav", 1);
        playSound("assets/sounds/foly/bb_char/take_hit_4.wav", 1);
    });
}

void SoundSystem::update(EntityManager& entityManager) {
    for (Entity& entity : entityManager.getEntities()) {
        if (entity.hasComponent<Sound>()) {
            Sound& sound = entity.getComponent<Sound>();
            playSound(sound.soundFile, 1);
        }
    }
}

void SoundSystem::playSound(const std::string& soundFile, int volumeDivisor) {
    Mix_Chunk* sound = Mix_LoadWAV(soundFile.c_str());
    if (sound == nullptr) {
        std::cout << "Sound: " << soundFile << " failed to load" << std::endl;
    }
    Mix_VolumeChunk(sound, MIX_MAX_VOLUME / volumeDivisor);
    Mix_PlayChannel(-1, sound, 0);
}

void SoundSystem::stopSound() {
    Mix_HaltChannel(-1);
}
