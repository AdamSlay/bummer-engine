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
    Jumps(int jumps, int maxJumps) : jumps(jumps), maxJumps(maxJumps) {}
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
    AnimationClip(SDL_Texture* spriteSheet, std::vector<SDL_Rect> frames, int framesPerImage, bool loop)
        : spriteSheet(spriteSheet), frames(frames), framesPerImage(framesPerImage), loop(loop) {}
};

struct Animator {
    std::map<playerStates, AnimationClip> animations;
    playerStates currentAnimation;
    int currentFrame;
    int currentImage;
    bool isPlaying;
    Animator(std::map<playerStates, AnimationClip> animations, playerStates currentAnimation, int currentFrame, int currentImage, bool isPlaying)
        : animations(animations), currentAnimation(currentAnimation), currentFrame(currentFrame), currentImage(currentImage), isPlaying(isPlaying) {}
};

struct State {
    playerStates state;
    State(playerStates state) : state(state) {}
};

struct Player {
    int playerNumber;
    Player(int playerNumber) : playerNumber(playerNumber) {}
};

struct Transform {
    int x, y;
    float scale;
    Transform(int x, int y, float scale) : x(x), y(y), scale(scale) {}
};

struct Velocity {
    int dx, dy, direction;  // 1 for right, -1 for left
    Velocity(int dx, int dy, int direction) : dx(dx), dy(dy), direction(direction) {}
};

struct Collider {
    int offsetX, offsetY, width, height;
    Collider(int offsetX, int offsetY, int width, int height)
        : offsetX(offsetX), offsetY(offsetY), width(width), height(height) {}
};

struct Scale {
    float scale;
    Scale(float scale) : scale(scale) {}
};

struct Sprite {
    SDL_Texture* texture;
    SDL_Rect srcRect;
    Sprite(SDL_Texture* texture, SDL_Rect srcRect) : texture(texture), srcRect(srcRect) {}
};

struct Gravity {
    float baseGravity;
    float gravity;
    float ascendFactor;
    float descendFactor;
    Gravity(float baseGravity, float gravity, float ascendFactor, float descendFactor)
        : baseGravity(baseGravity), gravity(gravity), ascendFactor(ascendFactor), descendFactor(descendFactor) {}
};

#endif //BUMMERENGINE_COMPONENTS_H