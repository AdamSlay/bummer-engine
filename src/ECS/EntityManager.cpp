#include "EntityManager.h"

template <typename T>
void Entity::addComponent(T component) {
    /**
     * Add a component of type T to the entity
     *
     * @param component The component to add
     */
    components[std::type_index(typeid(T))] = component;
}

template <typename T>
T& Entity::getComponent() {
    /**
     * Return a reference to the component of type T
     */
    return components[std::type_index(typeid(T))];
}

Entity& EntityManager::createEntity() {
    /**
     * Create a new entity, append it to the entities vector and return a reference to it
     */
    entities.emplace_back();
    return entities.back();
}