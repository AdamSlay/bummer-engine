#include "EventManager.h"
#include <iostream>

EventManager& EventManager::getInstance() {
    static EventManager instance;
    return instance;
}

void EventManager::subscribe(const std::string &event, std::function<void(EventData)> callback)
{
    if (eventMap.find(event) == eventMap.end())
    {
        eventMap[event] = std::vector<std::function<void(EventData)>>();
    }
    eventMap[event].push_back(callback);
}

void EventManager::publish(const std::string& event, EventData data) {
    auto it = eventMap.find(event);
    if (it != eventMap.end()) {
        for (auto &callback : it->second) {
            callback(data);
//            std::cout << "Event: " << event << " published" << std::endl;
        }
    }
}