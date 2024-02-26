#include "AnimationSystem.h"


void AnimationSystem::update(EntityManager& entityManager, float deltaTime) {
    for (Entity& entity : entityManager.getEntities()) {
        if (entity.hasComponent<Animator>() && entity.hasComponent<Sprite>()) {
            Animator& animator = entity.getComponent<Animator>();
            Sprite& sprite = entity.getComponent<Sprite>();
            State& state = entity.getComponent<State>();
            playerStates currentState = state.state;
            AnimationClip& currentClip = animator.animations[currentState];

            if (animator.isPlaying) {
                if (animator.currentFrame != 0 && animator.currentFrame % currentClip.framesPerImage == 0) {
                    // Switch to the next image in the animation
                    animator.currentImage++;
                    if (animator.currentImage >= currentClip.frames.size()) {
                        if (currentClip.loop) {
                            animator.currentImage = 0;
                        } else {
                            animator.currentImage = currentClip.frames.size() - 1;
                            animator.isPlaying = false;
                        }
                    }
                }
                sprite.texture = currentClip.spriteSheet;
                sprite.srcRect = currentClip.frames[animator.currentImage];
                animator.currentFrame++;
            }
        }
    }
}