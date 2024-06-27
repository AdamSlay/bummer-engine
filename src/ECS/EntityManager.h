#pragma once
#ifndef BUMMERENGINE_ENTITYMANAGER_H
#define BUMMERENGINE_ENTITYMANAGER_H

#include <unordered_map>
#include <typeindex>
#include <vector>

#include <nlohmann/json.hpp>

#include "../Resources/TextureManager.h"
#include "../ECS/Components.h"
#include "Entity.h"


class EntityManager {
public:
    std::vector<Entity> collidableEntities;
    std::vector<Entity> movableCollidableEntities;

    EntityManager(TextureManager* textureManager, SDL_Renderer* renderer);
    Entity& createEntity();
    void removeEntity(int entityId);
    std::vector<Entity>& getEntities();
    Entity& createEntityFromTemplate(const std::string& templatePath);
    void clearEntities();
    std::vector<Entity>& getCollidableEntities();
    std::vector<Entity>& getMovableCollidableEntities();
    Entity& createPlayer(int x, int y, int w, int h);
    void configureAnimator(Entity& entity, std::map<playerState, AnimationClip>& animations);
    Entity& getPlayer();
    Entity& getEntityById(int id);

private:
    static std::map<std::string, playerState> playerStatesMap;
    static std::map<std::string, Action> actionMap;
    std::vector<Entity> entities;
    TextureManager* textureManager;
    SDL_Renderer* renderer;

    void addComponentPlayer(Entity& entity, const nlohmann::json& componentJson);
    void addComponentNpc(Entity& entity, const nlohmann::json& componentJson);
    void addComponentIntent(Entity& entity, const nlohmann::json& componentJson);
    void addComponentTransform(Entity& entity, const nlohmann::json& componentJson);
    void addComponentSprite(Entity& entity, const nlohmann::json& componentJson);
    void addComponentCollider(Entity& entity, const nlohmann::json& componentJson);
};

#endif // BUMMERENGINE_ENTITYMANAGER_H