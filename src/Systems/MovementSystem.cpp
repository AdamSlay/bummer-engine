#include "MovementSystem.h"

#include <algorithm>
#include <iostream>

#include "../ECS/Components.h"
#include "../ECS/EventManager.h"
#include "../ECS/StateMachine.h"
#include "../Utils.h"

void MovementSystem::handleIntent(EntityManager& entityManager, float deltaTime){
    for (Entity& entity: entityManager.getEntities()) {
        if (entity.hasComponent<Intent>()) {
            Intent &intent = entity.getComponent<Intent>();
            Velocity &velocity = entity.getComponent<Velocity>();


            if (StateMachine::canMove(entity)) {

                // handle movement in x direction
                if (intent.direction == Direction::LEFT) {
                    EventManager::getInstance().publish("moveLeft", {&entity});
                    velocity.dx = -velocity.speed;
                } else if (intent.direction == Direction::RIGHT) {
                    EventManager::getInstance().publish("moveRight", {&entity});
                    velocity.dx = velocity.speed;
                } else {
                    EventManager::getInstance().publish("idle", {&entity});
                    velocity.dx = 0;
                }
                if (velocity.dx != 0) {
                    velocity.direction = (velocity.dx > 0) ? 1 : -1;
                }

                // handle movement in y direction
                if (intent.action == Action::JUMP) {
                    // jump(entity);
                    velocity.dy = -velocity.speed;
                }
                if (intent.action == Action::DOWN) {
                    velocity.dy = velocity.speed;
                }
                if (intent.action == Action::STOP_JUMP || intent.action == Action::STOP_DOWN) {
                    velocity.dy = 0;
                }
                // if (velocity.dy != 0) {
                //     EventManager::getInstance().publish("airborne", {&entity});
                // }

                // handle dash
                if (intent.action == Action::DASH) {
                    dash(entity, deltaTime);
                }
            }

            else if (entity.getComponent<State>().state == playerState::DASHING){
                dash(entity, deltaTime);
            }
        }
    }
}

void MovementSystem::move(EntityManager& entityManager) {
    /**
     * Iterate through all entities with a velocity component and move them
     */
    for (Entity& entity : entityManager.getEntities()) {
        if (entity.hasComponent<Velocity>()) {
            if (entity.hasComponent<Gravity>()) {
                applyGravity(entity);
            }
            if (entity.hasComponent<Transform>()) {
                Transform &transform = entity.getComponent<Transform>();
                Velocity &velocity = entity.getComponent<Velocity>();
                transform.x += velocity.dx;
                transform.y += velocity.dy;
            }
        }
    }
}

void MovementSystem::jump(Entity& entity) {
    /**
     * Make the entity jump
     *
     * @param entity: The entity
     */
    if (entity.hasComponent<Velocity>() && entity.hasComponent<Gravity>() && entity.hasComponent<Jumps>()) {
        Velocity& velocity = entity.getComponent<Velocity>();
        Gravity& gravity = entity.getComponent<Gravity>();
        Jumps& jumps = entity.getComponent<Jumps>();

        if (jumps.jumps != jumps.maxJumps) {
            jumps.jumps++;
            gravity.gravity = gravity.baseGravity;
            velocity.dy = -jumps.jumpVelocity;
            EventManager::getInstance().publish("jump", {&entity});
        }
    }
}

void MovementSystem::dash(Entity& entity, float deltaTime) {
    if (entity.hasComponent<Dash>() && entity.hasComponent<Velocity>()) {
        Input& input = entity.getComponent<Input>();
        Dash& dash = entity.getComponent<Dash>();
        Velocity& velocity = entity.getComponent<Velocity>();
        if (!dash.isDashing && dash.currentCooldown <= 0) {
            dash.isDashing = true;
            EventManager::getInstance().publish("dash", {&entity});
            // Set a specific velocity for the dash
            // TODO: magic numbers
            if (std::abs(input.joystickDirection.first) > 0.2 || std::abs(input.joystickDirection.second) > 0.2) {
                // If the joystick is being used, dash in the direction of the joystick
                std::cout << "Input value: " << input.joystickDirection.first << ", " << input.joystickDirection.second << std::endl;
                velocity.dx = input.joystickDirection.first * dash.speed;
                velocity.dy = input.joystickDirection.second * dash.speed;
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
                if (dx == 0 && dy == 0) {
                    dx = velocity.direction;
                }
                velocity.dx = dx * dash.speed;
                velocity.dy = dy * dash.speed;
            }
        }
        if (dash.isDashing) {
            dash.currentDuration -= deltaTime;
            if (dash.currentDuration <= 0) {
                EventManager::getInstance().publish("dashEnd", {&entity});
                dash.isDashing = false;
                dash.currentCooldown = dash.initCooldown; // Reset cooldown
                dash.currentDuration = dash.initDuration; // Reset duration
                velocity.dx /= dash.speed; // Reset velocity
                velocity.dy /= dash.speed;
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
    Velocity &velocity = entity.getComponent<Velocity>();
    Gravity &gravity = entity.getComponent<Gravity>();
    if (velocity.dy < 0) {
        gravity.gravity = std::max(gravity.gravity * gravity.ascendFactor, gravity.ascendMin);
    }
    else {
        gravity.gravity = std::min(gravity.gravity * gravity.descendFactor, gravity.descendMax);
    }
    // Apply gravity
    velocity.dy += gravity.gravity;
}
