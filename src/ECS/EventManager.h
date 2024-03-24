#ifndef BUMMERENGINE_EVENTMANAGER_H
#define BUMMERENGINE_EVENTMANAGER_H

#include <map>
#include <functional>
#include <vector>
#include <string>

#include "EntityManager.h"

struct EventData {
    int entityId = -1; // Default to -1 to indicate no specific entity
    Entity* primaryEntity = nullptr;
    Entity* secondaryEntity = nullptr;
    // Add other fields as needed
};

class EventManager {
public:

    static EventManager& getInstance();
    void subscribe(const std::string& event, std::function<void(EventData)> callback);
    void publish(const std::string& event, EventData data);

private:
    std::map<std::string, std::vector<std::function<void(EventData)>>> eventMap;
};

#endif //BUMMERENGINE_EVENTMANAGER_H