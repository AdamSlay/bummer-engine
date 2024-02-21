#include "EntityManager.h"

template <typename T>
void Entity::addComponent(T component) {
    components[std::type_index(typeid(T))] = component;
}

template <typename T>
T& Entity::getComponent() {
    return components[std::type_index(typeid(T))];
}

Entity& EntityManager::createEntity() {
    entities.emplace_back();
    return entities.back();
}