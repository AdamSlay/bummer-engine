#pragma once
#ifndef BUMMERENGINE_COMPONENTS_H
#define BUMMERENGINE_COMPONENTS_H

#include <map>

#include <SDL2/SDL.h>

enum class playerState
{
    IDLE,
    FLYING,
    GROUNDED,
    RUN,
    JUMP_ASCEND,
    JUMP_DESCEND,
    JUMP_APEX,
    JUMP_APEX_DESCEND,
    JUMP_APEX_ASCEND,
    HIT,
    STUNNED,
    DASHING,
    BASIC_ATTACK
};

enum class Action
{
    JUMP,
    DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
    ATTACK,
    WAIT,
    DASH,
    STOP_DOWN,
    STOP_JUMP
};

enum class Direction
{
    STILL,
    LEFT,
    RIGHT,
    JOY_LEFT,
    JOY_RIGHT
};

struct Sound
{
    std::string soundFile;
    Sound(std::string soundFile) : soundFile(soundFile) {}
};

struct Input
{
    std::map<SDL_Scancode, bool> keyStates;
    std::map<SDL_Scancode, bool> justPressed;
    std::map<SDL_Scancode, bool> justReleased;
    std::map<Action, bool> actionInput;
    std::pair<float, float> joystickDirection;
    Input(const std::map<SDL_Scancode, bool> &keyStates,
          const std::map<SDL_Scancode, bool> &justPressed,
          const std::map<SDL_Scancode, bool> &justReleased,
          const std::map<Action, bool> &actionInput)
          : keyStates(keyStates), justPressed(justPressed), justReleased(justReleased), actionInput(actionInput), joystickDirection(std::make_pair(0.0f, 0.0f)) {}
};

struct Intent
{
    Action action;
    Direction direction;
    Intent(Action action, Direction direction) : action(action), direction(direction) {};
};

struct Jumps
{
    int jumps;
    int maxJumps;
    int jumpVelocity;
    Jumps(int jumps, int maxJumps, int jumpVelocity) : jumps(jumps), maxJumps(maxJumps), jumpVelocity(jumpVelocity) {}
};

struct Health
{
    int maxHealth;
    int currentHealth;
    int invincibilityFrames; // Duration of damage cooldown (frames)
    int invincibilityRemaining; // Time until entity can take damage again (frames)
    Health(int maxHealth, int currentHealth, int invincibilityFrames)
        : maxHealth(maxHealth), currentHealth(currentHealth), invincibilityFrames(invincibilityFrames), invincibilityRemaining(0) {}
};

struct Dash
{
    int speed;
    bool isDashing;
    float initDuration;
    float initCooldown;
    float currentDuration;
    float currentCooldown;
    Dash(int speed, bool isDashing, float initDuration, float initCooldown) :
            speed(speed),
            isDashing(isDashing),
            initDuration(initDuration),
            initCooldown(initCooldown),
            currentDuration(initDuration),
            currentCooldown(0) {}
};

struct AnimationClip
{
    /**
     * A struct representing an animation clip
     * It contains a vector of SDL_Textures
     * representing the frames of the animation
     * This struct might be used to hold the running animation
     */
    SDL_Texture *spriteSheet;
    std::vector<SDL_Rect> frames;
    int framesPerImage;
    bool loop;
    std::string spritePath;
    AnimationClip(SDL_Texture *spriteSheet, std::vector<SDL_Rect> frames, int framesPerImage, bool loop, std::string spritePath)
        : spriteSheet(spriteSheet), frames(frames), framesPerImage(framesPerImage), loop(loop), spritePath(spritePath) {}
};

struct Animator
{
    std::map<playerState, AnimationClip> animations;
    playerState currentAnimation;
    int currentFrame;
    int currentImage;
    bool isPlaying;
    Animator(std::map<playerState, AnimationClip> animations, playerState currentAnimation, int currentFrame, int currentImage, bool isPlaying)
        : animations(animations), currentAnimation(currentAnimation), currentFrame(currentFrame), currentImage(currentImage), isPlaying(isPlaying) {}
};

struct State
{
    playerState state;
    State(playerState state) : state(state) {}
};

struct Player
{
    int playerNumber;
    Player(int playerNumber) : playerNumber(playerNumber) {}
};

struct Npc
{
    std::string type;
    Npc(std::string type) : type(type) {}
};

struct Transform
{
    int x, y;
    float scale;
    Transform(int x, int y, float scale) : x(x), y(y), scale(scale) {}
};

struct Velocity
{
    int dx, dy, direction, speed; // 1 for right, -1 for left
    Velocity(int dx, int dy, int direction, int speed) : dx(dx), dy(dy), direction(direction), speed(speed) {}
};

struct Collider
{
    int offsetX, offsetY, width, height;
    Collider(int offsetX, int offsetY, int width, int height)
        : offsetX(offsetX), offsetY(offsetY), width(width), height(height) {}
};

struct Sprite
{
    SDL_Texture *texture;
    SDL_Rect srcRect;
    Sprite(SDL_Texture *texture, SDL_Rect srcRect) : texture(texture), srcRect(srcRect) {}
};

struct Gravity
{
    float baseGravity;
    float gravity;
    float ascendFactor;
    float descendFactor;
    float ascendMin;
    float descendMax;
    Gravity(float baseGravity, float gravity, float ascendFactor, float descendFactor, float ascendMin, float descendMax)
        : baseGravity(baseGravity), gravity(gravity), ascendFactor(ascendFactor), descendFactor(descendFactor), ascendMin(ascendMin), descendMax(descendMax) {}
};

struct Hitbox
{
    int offsetX, offsetY, width, height;
    Hitbox(int offsetX, int offsetY, int width, int height)
            : offsetX(offsetX), offsetY(offsetY), width(width), height(height) {}
    Hitbox() : offsetX(0), offsetY(0), width(0), height(0) {} // Default constructor
};

struct AttackInfo
{
    int damage;
    bool isActive;
    Hitbox hitbox;
    int knockback;
    int duration; // in frames
    int windupFrames;
    int frameCounter; // counts the number of frames since the attack was activated
    AttackInfo(int damage, bool isActive, Hitbox hitbox, int knockback, int duration, int windupFrames)
            : damage(damage), isActive(isActive), hitbox(hitbox), knockback(knockback), duration(duration), windupFrames(windupFrames), frameCounter(0) {}
    AttackInfo() : damage(0), isActive(false), hitbox(Hitbox()), knockback(0), duration(0), windupFrames(0), frameCounter(0) {} // Default constructor
};
struct AttackMap
{
    std::map<std::string, AttackInfo> attacks;
};

struct AI
{
    std::string state;
    std::pair<int, int> patrolStart;
    float patrolRange;
    float attackRange;
    float pursuitRange;
    AI(std::string state, std::pair<int, int> patrolStart, float patrolRange, float attackRange, float pursuitRange)
        : state(state), patrolStart(patrolStart), patrolRange(patrolRange), attackRange(attackRange), pursuitRange(pursuitRange) {}
};


#endif // BUMMERENGINE_COMPONENTS_H