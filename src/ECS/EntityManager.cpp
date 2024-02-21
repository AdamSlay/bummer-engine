#ifndef BUMMERENGINE_ENTITYMANAGER_H
#define BUMMERENGINE_ENTITYMANAGER_H

#include <vector>
#include <unordered_map>
#include <typeindex>

class Entity
{
public:
    template <typename T>
    void addComponent(T component)
    {
        components[std::type_index(typeid(T))] = component;
    }

    template <typename T>
    T& getComponent()
    {
        return components[std::type_index(typeid(T))];
    }

private:
    std::unordered_map<std::type_index, void *> components;
};

class EntityManager
{
public:
    Entity& createEntity()
    {
        entities.emplace_back();  // similar to push_back, but constructs the object in place
        return entities.back();
    }

private:
    std::vector<Entity> entities;
};

#endif // BUMMERENGINE_ENTITYMANAGER_H