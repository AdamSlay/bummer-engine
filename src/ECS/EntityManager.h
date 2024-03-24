#ifndef BUMMERENGINE_ENTITYMANAGER_H
#define BUMMERENGINE_ENTITYMANAGER_H

#include <unordered_map>
#include <typeindex>
#include <vector>
#include "../Resources/TextureManager.h"
#include "../ECS/Components.h"

class Entity
{
public:
    Entity();
    bool operator==(const Entity& other) const;
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
    const T &getComponent() const {
        /**
         * Get a reference to the component of type T
         *
         * this function is used when the entity is const
        */
        return *static_cast<const T *>(components.at(std::type_index(typeid(T))));
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
    void changeState(playerStates newState) {
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

    int getID() const {
        /**
         * Get the ID of the entity
         */
        return id;
    }

private:
    int id;  // Unique identifier for each entity
    static int nextId;  // Static member to generate unique IDs
    std::unordered_map<std::type_index, void *> components;
};

class EntityManager {
public:
    EntityManager(TextureManager* texManager, SDL_Renderer* ecsRenderer);
    Entity& createEntity();
    void removeEntity(int entityId);
    std::vector<Entity>& getEntities();
    Entity& createEntityFromTemplate(const std::string& templatePath);
    void clearEntities();
    Entity& createPlayer(int x, int y, int w, int h);
    void configureAnimator(Entity& entity, std::map<playerStates, AnimationClip>& animations);
    Entity& getPlayer();
    Entity& getEntityById(int id);

private:
    static std::map<std::string, playerStates> playerStatesMap;
    std::vector<Entity> entities;
    TextureManager* textureManager;
    SDL_Renderer* renderer;
};

#endif // BUMMERENGINE_ENTITYMANAGER_H