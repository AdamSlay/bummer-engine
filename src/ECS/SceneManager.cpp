#include "SceneManager.h"

#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;



SceneManager::SceneManager(EntityManager& entityManager) : entityManager(entityManager) {
    // Create sceneTemplates from run_config.json
    std::ifstream file("etc/run_config.json");
    json configJson;
    file >> configJson;

    json templates = configJson["SCENE_TEMPLATES"];
    for (auto& [key, value] : templates.items()) {
        sceneTemplates.push_back(value);
    }
};

void SceneManager::nextScene() {
    currentSceneIndex++;

    if (currentSceneIndex >= sceneTemplates.size()) {
        currentSceneIndex = 0;
    }
    entityManager.clearEntities();
    loadSceneFromTemplate(sceneTemplates[currentSceneIndex]);
}

void SceneManager::loadSceneFromTemplate(const std::string& templatePath) {
    // Load the template file
    std::ifstream file(templatePath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open scene template file: " + templatePath);
    }

    // Parse the JSON
    json templateJson;
    file >> templateJson;

    // For each entity in the template, create the entity
    for (const auto& entityTemplate : templateJson["entities"]) {
        entityManager.createEntityFromTemplate(entityTemplate["templatePath"]);
    }
}


