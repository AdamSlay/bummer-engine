#ifndef BUMMERENGINE_COMPONENTS_H
#define BUMMERENGINE_COMPONENTS_H

#include <map>

#include <SDL2/SDL.h>

enum class playerStates {
    IDLE,
    GROUNDED,
    RUN,
    JUMP_ASCEND,
    JUMP_DESCEND,
    JUMP_APEX,
    JUMP_APEX_DESCEND,
    JUMP_APEX_ASCEND,
    DOUBLE_JUMP,
    FALL
};

struct Jumps {
    int jumps;
    int maxJumps;
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
};

struct Player {
    int playerNumber;
};

struct Position {
    int x, y;
};

struct Velocity {
    int dx, dy, direction;  // 1 for right, -1 for left
};

struct Collider {
    int offsetX, offsetY, width, height;
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