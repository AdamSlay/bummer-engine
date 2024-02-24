#ifndef BUMMERENGINE_TEXTUREMANAGER_H
#define BUMMERENGINE_TEXTUREMANAGER_H

#include <string>
#include <unordered_map>

#include <SDL2/SDL.h>


class TextureManager {
public:
    SDL_Texture* loadTexture(SDL_Renderer* renderer, const std::string& filePath);
    void freeTexture(SDL_Texture* texture);
private:
    std::unordered_map<std::string, SDL_Texture*> textures;
};


#endif //BUMMERENGINE_TEXTUREMANAGER_H
