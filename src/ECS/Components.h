#ifndef BUMMERENGINE_COMPONENTS_H
#define BUMMERENGINE_COMPONENTS_H

#include <map>

#include <SDL2/SDL.h>

enum class playerStates {
    IDLE,
    GROUNDED,
    RUN,
    JUMP,
    DOUBLE_JUMP,
    FALL
};

struct AnimationClip {
    /**
     * A struct representing an animation clip
     * It contains a vector of SDL_Textures
     * representing the frames of the animation
     * This struct might be used to hold the running animation
     */
    SDL_Texture* spriteSheet;
    std::vector<SDL_Rect> frames;
    int framesPerImage;
    bool loop;
};

struct Animator {
    std::map<playerStates, AnimationClip> animations;
    playerStates currentAnimation;
    int currentFrame;
    int currentImage;
    bool isPlaying;
};

struct State {
    playerStates state;
    void changeState(playerStates newState, Animator& animator) {
        if (state != newState) {
            state = newState;
            animator.currentFrame = 0;
            animator.isPlaying = true;
        }
    }
};

struct Player {
    int playerNumber;
};

struct Position {
    int x, y;
};

struct Velocity {
    int dx, dy;
};

struct Collider {
    int width, height;
};

struct Scale {
    float scale;
};

struct Sprite {
    SDL_Texture* texture;
    SDL_Rect srcRect;
};

struct Gravity {
    float baseGravity;
    float gravity;
    float ascendFactor;
    float descendFactor;
};

#endif //BUMMERENGINE_COMPONENTS_H