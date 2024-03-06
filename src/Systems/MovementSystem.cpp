#include "MovementSystem.h"
#include <iostream>

#include "../ECS/Components.h"
#include "../ECS/EventManager.h"

void MovementSystem::moveX(EntityManager& entityManager, float deltaTime) {
    for (Entity& entity : entityManager.getEntities()) {
        if (entity.hasComponent<Transform>() && entity.hasComponent<Velocity>() && entity.hasComponent<Input>()) {
            Transform& pos = entity.getComponent<Transform>();
            Velocity& vel = entity.getComponent<Velocity>();
            Input& input = entity.getComponent<Input>();

            dash(entity, deltaTime);
            Dash& dash = entity.getComponent<Dash>();
            if (!dash.isDashing) {
                if (input.keyStates[SDL_SCANCODE_LEFT]) {
                    vel.dx = -5;
                }
                else if (input.keyStates[SDL_SCANCODE_RIGHT]) {
                    vel.dx = 5;
                }
                else {
                    vel.dx = 0;
                }
                if (vel.dx != 0) {
                    vel.direction = (vel.dx > 0) ? 1 : -1;
                }
            }
            pos.x += vel.dx;
        }
    }
}

void MovementSystem::moveY(EntityManager &entityManager) {
    for (Entity &entity : entityManager.getEntities()) {
        if (entity.hasComponent<Transform>() && entity.hasComponent<Velocity>() && entity.hasComponent<Input>()) {
            Velocity &vel = entity.getComponent<Velocity>();
            Input& input = entity.getComponent<Input>();
            if (input.justPressed[SDL_SCANCODE_UP]) {
                jump(entity);
            }
            if (input.justReleased[SDL_SCANCODE_UP] && vel.dy < 0) {  // If the jump button is released while ascending, stop ascending
                vel.dy = 0;
            }
            if (vel.dy != 0) {
                changeJumpState(entity);
            }
            if (entity.hasComponent<Gravity>()) {
                if (entity.hasComponent<Dash>()) {
                    Dash& dash = entity.getComponent<Dash>();
                    if (!dash.isDashing) {
                        applyGravity(entity);
                    }
                    else {
                        // If the entity is dashing, don't apply gravity
                        Gravity& gravity = entity.getComponent<Gravity>();
                        gravity.gravity = 0.5;
                    }
                }
                else {
                    applyGravity(entity);
                }
            }
            // Apply velocity
            Transform &pos = entity.getComponent<Transform>();
            pos.y += vel.dy;
        }
    }
}

void MovementSystem::jump(Entity& entity) {
    /**
     * Make the entity jump
     *
     * @param entity: The entity
     */
    if (entity.hasComponent<Velocity>() && entity.hasComponent<State>()) {
        Velocity& vel = entity.getComponent<Velocity>();
        State& state = entity.getComponent<State>();
        Gravity& gravity = entity.getComponent<Gravity>();
        Jumps& jumps = entity.getComponent<Jumps>();

        if (jumps.jumps != jumps.maxJumps) {
            jumps.jumps++;
            gravity.gravity = gravity.baseGravity;
            // TODO: Magic number
            vel.dy = -17;
            entity.changeState(playerStates::JUMP_ASCEND);
            EventManager::getInstance().publish("jump");
        }
    }
}

void MovementSystem::dash(Entity& entity, float deltaTime) {
    if (entity.hasComponent<Input>() && entity.hasComponent<Dash>() && entity.hasComponent<Velocity>()) {
        Input& input = entity.getComponent<Input>();
        Dash& dash = entity.getComponent<Dash>();
        Velocity& vel = entity.getComponent<Velocity>();
        if (input.justPressed[SDL_SCANCODE_SPACE] && !dash.isDashing && dash.currentCooldown <= 0) {
            dash.isDashing = true;
            EventManager::getInstance().publish("dash");
            // Set a specific velocity for the dash
            if (std::abs(input.joystickDirection.first) > 0.2 || std::abs(input.joystickDirection.second) > 0.2) {
                // If the joystick is being used, dash in the direction of the joystick
                std::cout << "Input value: " << input.joystickDirection.first << ", " << input.joystickDirection.second << std::endl;
                vel.dx = input.joystickDirection.first * dash.speed;
                vel.dy = input.joystickDirection.second * dash.speed;
            } else {
                // If the joystick is not being used, dash in the direction of the keyboard input
                float dx = 0.0f, dy = 0.0f;
                if (input.keyStates[SDL_SCANCODE_UP]) dy = -1.0f;
                if (input.keyStates[SDL_SCANCODE_DOWN]) dy = 1.0f;
                if (input.keyStates[SDL_SCANCODE_LEFT]) dx = -1.0f;
                if (input.keyStates[SDL_SCANCODE_RIGHT]) dx = 1.0f;
                // Normalize the direction vector
                float length = std::sqrt(dx * dx + dy * dy);
                if (length > 0) {
                    dx /= length;
                    dy /= length;
                }
                vel.dx = dx * dash.speed;
                vel.dy = dy * dash.speed;
            }
        }
        if (dash.isDashing) {
            dash.currentDuration -= deltaTime;
            if (dash.currentDuration <= 0) {
                dash.isDashing = false;
                dash.currentCooldown = dash.initCooldown; // Reset cooldown
                dash.currentDuration = dash.initDuration; // Reset duration
                vel.dx /= dash.speed; // Reset velocity
                vel.dy /= dash.speed;
            }
        } else if (dash.currentCooldown > 0) {
            dash.currentCooldown -= deltaTime;
            if (dash.currentCooldown < 0) {
                std::cout << "Cooldown over" << std::endl;
                dash.currentCooldown = 0;
            }
        }
    }
}

void MovementSystem::applyGravity(Entity &entity) {
    /**
     * Apply gravity to the entity
     *
     * @param entity: The entity
     */
    if (entity.hasComponent<Velocity>() && entity.hasComponent<Gravity>()) {
        Velocity &vel = entity.getComponent<Velocity>();
        Gravity &gravity = entity.getComponent<Gravity>();
        if (vel.dy < 0) {
            gravity.gravity *= gravity.ascendFactor; // Decrease gravity when ascending
            if (gravity.gravity < 0.65) {
                gravity.gravity = 0.65;
            }
        }
        else {
            gravity.gravity *= gravity.descendFactor; // Increase gravity when descending
        }
        // Apply gravity
        vel.dy += gravity.gravity;
    }
}

void MovementSystem::changeJumpState(Entity &entity) {
    /**
     * Change the jump state of the entity
     *
     * @param entity: The entity
     */
    if (entity.hasComponent<Velocity>()) {
        Velocity &vel = entity.getComponent<Velocity>();
        if (vel.dy < -3) {
            entity.changeState(playerStates::JUMP_ASCEND);
        }
        else if (vel.dy < -1) {
            entity.changeState(playerStates::JUMP_APEX_ASCEND);
        }
        else if (vel.dy < 0) {
            entity.changeState(playerStates::JUMP_APEX);
        }
        else if (0 < vel.dy && vel.dy < 2) {
            entity.changeState(playerStates::JUMP_APEX);
        }
        else if (0 < vel.dy && vel.dy < 5) {
            entity.changeState(playerStates::JUMP_APEX_DESCEND);
        }
        else if (vel.dy >= 5) {
            entity.changeState(playerStates::JUMP_DESCEND);
        }
    }
}
