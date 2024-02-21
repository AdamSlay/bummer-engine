#ifndef BUMMERENGINE_ENTITYMANAGER_H
#define BUMMERENGINE_ENTITYMANAGER_H

#include <unordered_map>
#include <typeindex>

class Entity {
public:
    template <typename T>
    void addComponent(T component);

    template <typename T>
    T& getComponent();

private:
    std::unordered_map<std::type_index, void *> components;
};


class EntityManager {
public:
    Entity& createEntity();

private:
    std::vector<Entity> entities;
};

#endif // BUMMERENGINE_ENTITYMANAGER_H