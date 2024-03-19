#include "SoundSystem.h"
#include "../ECS/EventManager.h"


SoundSystem::SoundSystem() {
    EventManager::getInstance().subscribe("jump", [this]() {
        playSound("assets/sounds/zapsplat/zapsplat_multimedia_game_jump_24928.wav", 6);
    });
    EventManager::getInstance().subscribe("died", [this]() {
        playSound("assets/sounds/zapsplat/zapsplat_impacts_body_hit_thud_stab_squelch_of_blood_90708.wav", 2);
    });
    EventManager::getInstance().subscribe("spawn", [this]() {
        playSound("assets/sounds/zapsplat/zapsplat_sound_design_rewind_reversed_vibration_001_19653.wav", 2);
    });
    EventManager::getInstance().subscribe("dash", [this]() {
        playSound("assets/sounds/zapsplat/zapsplat_cartoon_whoosh_swipe_fast_grab_dash_006_74747.wav", 4);
    });
    EventManager::getInstance().subscribe("start", [this]() {
        playSound("assets/sounds/volts-theme.wav", 4);
    });
    EventManager::getInstance().subscribe("landed", [this]() {
        playSound("assets/sounds/zapsplat/foley_clothing_coat_land_floor_001.wav", 4);
    });
    EventManager::getInstance().subscribe("basicAttack", [this]() {
        playSound("assets/sounds/zapsplat/tennis_fast_whoosh_swing_006_106738.wav", 4);
    });
    EventManager::getInstance().subscribe("enemyHit", [this]() {
        playSound("assets/sounds/zapsplat/voice_high_pitched_pain_grunt_003_15785.wav", 4);
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
        // Handle error
    }
    Mix_VolumeChunk(sound, MIX_MAX_VOLUME / volumeDivisor);
    Mix_PlayChannel(-1, sound, 0);
}

void SoundSystem::stopSound() {
    Mix_HaltChannel(-1);
}
