#include <unordered_map>
#include <typeindex>
#include <vector>
#include "../Resources/TextureManager.h"
#include "../ECS/Components.h"

#include "Entity.h"

int Entity::nextId = 0;

Entity::Entity() {
    /**
     * Constructor for the Entity
     */
    id = nextId++;
}

bool Entity::operator==(const Entity& other) const {
    return this->id == other.id;
}

void Entity::changeState(playerStates newState) {
    /**
     * Change the state of the entity
     *
     * @param newState: The new state
     */
    if (getComponent<State>().state != newState) {
        getComponent<State>().state = newState;
        Animator& animator = getComponent<Animator>();
        animator.currentFrame = 0;
        animator.currentImage = 0;
        animator.isPlaying = true;
    }
}

void Entity::resetIntent(bool direction) {
    /**
     * Reset the intent of the entity
     */
    Intent& intent = getComponent<Intent>();
    intent.action = Action::WAIT;
    if (direction) {
        intent.direction = Direction::STILL;
    }
}

int Entity::getID() const {
    /**
     * Get the ID of the entity
     */
    return id;
}

void Entity::setTransformPos(int x, int y) {
    /**
     * Set the x and y position of the transform, adjusting for the collider offset and scale
     *
     * @param x: The x position
     * @param y: The y position
     * @throws runtime_error if entity does not have required components: Transform or Collider
     */
    if (!this->hasComponent<Transform>() || !this->hasComponent<Collider>()) {
        throw std::runtime_error("Entity does not have required components: Transform or Collider");
    }

    this->setTransformX(x);
    this->setTransformY(y);
}

void Entity::setTransformX(int x) {
    /**
     * Set the x position of the transform, adjusting for the collider offset and scale
     *
     * @param x: The x position
     * @throws runtime_error if entity does not have required components: Transform or Collider
     */

    Transform& transform = this->getComponent<Transform>();
    const Collider& collider = this->getComponent<Collider>();
    transform.x = x - (collider.offsetX * transform.scale);
}

void Entity::setTransformY(int y) {
    /**
     * Set the y position of the transform, adjusting for the collider offset and scale
     *
     * @param y: The y position
     * @throws runtime_error if entity does not have required components: Transform or Collider
     */

    Transform& transform = this->getComponent<Transform>();
    const Collider& collider = this->getComponent<Collider>();
    transform.y = y - (collider.offsetY * transform.scale);
}

SDL_Rect Entity::getColliderRect() {
    /**
     * Get the position of the Entity's collider
     *
     * @return: SDL_Rect representing the collider position
     */
    if (!this->hasComponent<Transform>() || !this->hasComponent<Collider>()) {
        throw std::runtime_error("Entity does not have required components: Transform or Collider");
    }
    const auto transform = this->getComponent<Transform>();
    const auto collider = this->getComponent<Collider>();

    SDL_Rect colliderRect = calculateColliderRect();
    return colliderRect;
}

SDL_Rect Entity::calculateColliderRect() {
    /**
     * Calculate the position of the collider based on the transform
     *
     * @param transform: The transform
     * @param collider: The collider
     * @return: SDL_Rect representing the collider position
     */
    auto& transform = this->getComponent<Transform>();
    auto& collider = this->getComponent<Collider>();
    int xPosition = transform.x + (collider.offsetX * transform.scale);
    int yPosition = transform.y + (collider.offsetY * transform.scale);
    int width = collider.width * transform.scale;
    int height = collider.height * transform.scale;
    return SDL_Rect {xPosition, yPosition, width, height};
}

SDL_Rect Entity::getHitboxRect(Hitbox& hitbox) {
    /**
     * Get the position of the hitbox relative to entity collider
     *
     * @param hitbox: The hitbox
     * @param entity: The entity
     * @return: A pair of integers representing the x and y position of the hitbox
     */

    SDL_Rect hitboxRect;
    SDL_Rect playerCollider = this->getColliderRect();

    int direction = this->getComponent<Velocity>().direction;
    if (direction == 1) { // player is facing right
        hitboxRect.x = playerCollider.x + hitbox.offsetX + playerCollider.w;
    } else { // player is facing left
        hitboxRect.x = playerCollider.x - hitbox.offsetX - hitbox.width;
    }
    hitboxRect.y = playerCollider.y + hitbox.offsetY;
    hitboxRect.w = hitbox.width;
    hitboxRect.h = hitbox.height;

    return hitboxRect;
}


