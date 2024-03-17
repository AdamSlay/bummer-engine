#ifndef BUMMERENGINE_SCENEMANAGER_H
#define BUMMERENGINE_SCENEMANAGER_H

#include <vector>

#include "EntityManager.h"

class SceneManager {
public:
    SceneManager(EntityManager& entityManager);
    void loadSceneFromTemplate(const std::string& sceneTemplate);

private:
    EntityManager& entityManager;
    std::vector<std::string> sceneTemplates;
    int currentSceneIndex = 0;
};

#endif //BUMMERENGINE_SCENEMANAGER_H
