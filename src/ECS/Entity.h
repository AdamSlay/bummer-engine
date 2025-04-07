#ifndef ENTITY_H
#define ENTITY_H

#include <unordered_map>
#include <typeindex>
#include <vector>
#include <stdexcept>
#include <SDL2/SDL.h>
#include "../Resources/TextureManager.h"
#include "../ECS/Components.h"

class Entity {
public:
    int id;

    Entity(int id);

    template <typename T>
    void addComponent(T component);

    template <typename T>
    T& getComponent();

    template <typename T>
    const T& getComponent() const;

    template <typename T>
    bool hasComponent() const;

    void changeState(playerState newState);
    void changeFlyingState(bool isFlying);
    void resetIntent(bool direction);
    int getID() const;
    void setTransformPos(int x, int y);
    void setTransformX(int x);
    void setTransformY(int y);
    SDL_Rect getColliderRect();
    SDL_Rect calculateColliderRect();
    SDL_Rect getHitboxRect(Hitbox& hitbox);
    bool operator==(const Entity& other) const {
        return getID() == other.getID();
    }

private:
    std::unordered_map<std::type_index, void*> components;
};

template <typename T>
void Entity::addComponent(T component) {
    components[std::type_index(typeid(T))] = new T(component);
}

template <typename T>
T& Entity::getComponent() {
    return *static_cast<T*>(components[std::type_index(typeid(T))]);
}

template <typename T>
const T& Entity::getComponent() const {
    return *static_cast<const T*>(components.at(std::type_index(typeid(T))));
}

template <typename T>
bool Entity::hasComponent() const {
    return components.find(std::type_index(typeid(T))) != components.end();
}

#endif // ENTITY_H
