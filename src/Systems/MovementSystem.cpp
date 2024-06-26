#include "MovementSystem.h"
#include <iostream>

#include "../ECS/Components.h"
#include "../ECS/EventManager.h"
#include "../ECS/StateMachine.h"
#include "../Utils.h"

void MovementSystem::handleIntent(EntityManager& entityManager, float deltaTime){
    for (Entity& entity: entityManager.getEntities()) {
        if (entity.hasComponent<Intent>()) {
            Intent &intent = entity.getComponent<Intent>();
            Velocity &vel = entity.getComponent<Velocity>();


            if (StateMachine::canMove(entity)) {

                // handle movement in x direction
                if (intent.direction == Direction::LEFT) {
                    vel.dx = -vel.speed;
                } else if (intent.direction == Direction::RIGHT) {
                    vel.dx = vel.speed;
                } else {
                    vel.dx = 0;
                }
                if (vel.dx != 0) {
                    vel.direction = (vel.dx > 0) ? 1 : -1;
                }

                // handle movement in y direction
                if (intent.action == Action::JUMP) {
                    jump(entity);
                }
                if (intent.action == Action::STOP_JUMP && vel.dy < 0) {  // If the jump button is released while ascending, stop ascending
                    vel.dy = 0;
                }
                if (vel.dy != 0) {
                    EventManager::getInstance().publish("airborne", {&entity});
                }

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

void MovementSystem::moveX(EntityManager& entityManager) {
    for (Entity& entity : entityManager.getEntities()) {
        if (entity.hasComponent<Transform>() && entity.hasComponent<Velocity>()) {
            Transform &pos = entity.getComponent<Transform>();
            Velocity &vel = entity.getComponent<Velocity>();
            pos.x += vel.dx;
        }
    }
}

void MovementSystem::moveY(EntityManager &entityManager) {
    for (Entity &entity : entityManager.getEntities()) {
        if (entity.hasComponent<Transform>() && entity.hasComponent<Velocity>()) {
            applyGravity(entity);

            Velocity &vel = entity.getComponent<Velocity>();
            Transform &transform = entity.getComponent<Transform>();
            transform.y += vel.dy;
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
        Gravity& gravity = entity.getComponent<Gravity>();
        Jumps& jumps = entity.getComponent<Jumps>();

        if (jumps.jumps != jumps.maxJumps) {
            jumps.jumps++;
            gravity.gravity = gravity.baseGravity;
            vel.dy = -jumps.jumpVelocity;
            EventManager::getInstance().publish("jump", {&entity});
        }
    }
}

void MovementSystem::dash(Entity& entity, float deltaTime) {
    if (entity.hasComponent<Dash>() && entity.hasComponent<Velocity>()) {
        Input& input = entity.getComponent<Input>();
        Dash& dash = entity.getComponent<Dash>();
        Velocity& vel = entity.getComponent<Velocity>();
        if (!dash.isDashing && dash.currentCooldown <= 0) {
            dash.isDashing = true;
            EventManager::getInstance().publish("dash", {&entity});
            // Set a specific velocity for the dash
            // TODO: magic numbers
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
                if (dx == 0 && dy == 0) {
                    dx = vel.direction;
                }
                vel.dx = dx * dash.speed;
                vel.dy = dy * dash.speed;
            }
        }
        if (dash.isDashing) {
            dash.currentDuration -= deltaTime;
            if (dash.currentDuration <= 0) {
                EventManager::getInstance().publish("dashEnd", {&entity});
                dash.isDashing = false;
                dash.currentCooldown = dash.initCooldown; // Reset cooldown
                dash.currentDuration = dash.initDuration; // Reset duration
                vel.dx /= dash.speed; // Reset velocity
                vel.dy /= dash.speed;
                Gravity& gravity = entity.getComponent<Gravity>();
                gravity.gravity = 0.5;
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
            if (gravity.gravity > 3) {
                gravity.gravity = 3;
            }
        }
        // Apply gravity
        vel.dy += gravity.gravity;
    }
}
