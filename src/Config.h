#ifndef BUMMERENGINE_CONFIG_H
#define BUMMERENGINE_CONFIG_H

#include <nlohmann/json.hpp>

class Config {
private:
    static Config* instance;
    nlohmann::json configData;

    Config();

public:
    static Config* getInstance();

    int getScreenWidth();

    int getScreenHeight();
};


#endif //BUMMERENGINE_CONFIG_H
