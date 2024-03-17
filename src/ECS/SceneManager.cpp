#include "SceneManager.h"

#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

SceneManager::SceneManager(EntityManager& entityManager) : entityManager(entityManager) {};


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


