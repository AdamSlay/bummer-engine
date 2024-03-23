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
            auto clip = animator.animations.find(state.state);
            if (clip != animator.animations.end()) {
                AnimationClip& currentClip = clip->second;
                if (animator.isPlaying) {
                    if (animator.currentFrame != 0 && animator.currentFrame % currentClip.framesPerImage == 0) {
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
                    animator.currentFrame++;
                }
            }
            else {
                std::cout << "Animation not found for state: " << Utils::playerStateToString(state.state) << std::endl;
            }
        }
    }
}