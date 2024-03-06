#ifndef BUMMERENGINE_EVENTMANAGER_H
#define BUMMERENGINE_EVENTMANAGER_H

#include <map>
#include <functional>
#include <vector>
#include <string>

class EventManager {
public:
    static EventManager& getInstance();
    void subscribe(const std::string& event, std::function<void()> callback);
    void publish(const std::string& event);

private:
    std::map<std::string, std::vector<std::function<void()>>> eventMap;
};

#endif //BUMMERENGINE_EVENTMANAGER_H