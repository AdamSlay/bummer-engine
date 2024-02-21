#ifndef BUMMERENGINE_ENTITYMANAGER_H
#define BUMMERENGINE_ENTITYMANAGER_H

#include <unordered_map>
#include <typeindex>

class Entity
{
public:
    template <typename T>
    void addComponent(T component)
    {
        components[std::type_index(typeid(T))] = new T(component);
    }

    template <typename T>
    T &getComponent()
    {
        return *static_cast<T *>(components[std::type_index(typeid(T))]);
    }

private:
    std::unordered_map<std::type_index, void *> components;
};

class EntityManager {
public:
    Entity& createEntity();
    Entity& createPlatform(int x, int y, int w, int h);

private:
    std::vector<Entity> entities;
};

#endif // BUMMERENGINE_ENTITYMANAGER_H