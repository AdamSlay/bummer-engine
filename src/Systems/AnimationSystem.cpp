#include "AnimationSystem.h"
#include "../Utils.h"

#include <iostream>

void AnimationSystem::update(EntityManager& entityManager, float deltaTime) {
    /**
     * Update the animation system
     *
     * @param entityManager: The entity manager
     * @param deltaTime: The time between frames
     */
    for (Entity& entity : entityManager.getEntities()) {
        if (entity.hasComponent<Animator>() && entity.hasComponent<Sprite>()) {
            Animator& animator = entity.getComponent<Animator>();
            Sprite& sprite = entity.getComponent<Sprite>();
            State& state = entity.getComponent<State>();
            playerStates currentState = state.state;
            auto clip = animator.animations.find(currentState);
            if (clip != animator.animations.end()) {
                AnimationClip& currentClip = clip->second;
                if (animator.isPlaying) {
                    if (state.state == playerStates::BASIC_ATTACK) {
                        std::cout << "Animator playing currentImage: " << animator.currentImage << std::endl;
                        std::cout << "Animator playing currentFrame: " << animator.currentFrame << std::endl;
                        std::cout << "currentClip.framesPerImage: " << currentClip.framesPerImage << std::endl;
                    }
                    if (animator.currentFrame != 0 && animator.currentFrame % currentClip.framesPerImage == 0) {
                        if (state.state == playerStates::BASIC_ATTACK) {
                            std::cout << "Advancing currentImage to: " << animator.currentImage << std::endl;
                        }
                        // Switch to the next image in the animation
                        animator.currentImage++;
                        if (animator.currentImage >= currentClip.frames.size()) {
                            if (currentClip.loop) {
                                animator.currentImage = 0;
                            } else {
                                animator.currentImage = currentClip.frames.size() - 1;
                                animator.isPlaying = false;  // gets reset in changeState()
                            }
                        }
                    }
                    sprite.texture = currentClip.spriteSheet;
                    sprite.srcRect = currentClip.frames[animator.currentImage];
                    if (state.state == playerStates::BASIC_ATTACK) {
                        std::cout << "iterating currentFrame: " << animator.currentFrame << std::endl;
                    }
                    animator.currentFrame++;
                    if (state.state == playerStates::BASIC_ATTACK) {
                        std::cout << "currentFrame iterated: " << animator.currentFrame << std::endl;
                    }
                }
            }
            else {
                std::cout << "Animation not found for state: " << Utils::playerStateToString(currentState) << std::endl;
            }
        }
    }
}