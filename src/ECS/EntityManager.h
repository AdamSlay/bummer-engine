#ifndef BUMMERENGINE_ENTITYMANAGER_H
#define BUMMERENGINE_ENTITYMANAGER_H

#include <unordered_map>
#include <typeindex>
#include <vector>
#include "../Resources/TextureManager.h"

class Entity
{
public:
    template <typename T>
    void addComponent(T component) {
        /**
         * Add a component of type T to the entity
        */
        components[std::type_index(typeid(T))] = new T(component);
    }

    template <typename T>
    T &getComponent() {
        /**
         * Get a reference to the component of type T
        */
        return *static_cast<T *>(components[std::type_index(typeid(T))]);
    }

    template <typename T>
    bool hasComponent() {
        /**
         * Check if the entity has a component of type T
        */
        // .find() is a member function of std::unordered_map that returns an iterator to the element if found
        // otherwise it returns an iterator to the position after the end of the map, ie components.end()
        // return true if the iterator is not equal to components.end(), meaning the component was found
        return components.find(std::type_index(typeid(T))) != components.end();
    }

private:
    std::unordered_map<std::type_index, void *> components;
};

class EntityManager {
public:
    Entity& createEntity();
    std::vector<Entity>& getEntities();
    Entity& createPlayer(int x, int y, int w, int h);
    Entity& createPlatform(int x, int y, int w, int h);
    void setTextureManager(TextureManager* texManager);
    void setRenderer(SDL_Renderer* ecsRenderer);
    Entity& getPlayer();

private:
    std::vector<Entity> entities;
    TextureManager* textureManager;
    SDL_Renderer* renderer;
};

#endif // BUMMERENGINE_ENTITYMANAGER_H