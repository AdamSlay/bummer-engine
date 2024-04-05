#include <iostream>

#include <SDL2/SDL_image.h>

#include "TextureManager.h"

SDL_Texture* TextureManager::loadTexture(SDL_Renderer* renderer, const std::string& filePath) {
    /**
     * Load a texture from a file and return it
     * If the texture is already loaded, return it
     *
     * @param renderer The renderer to use
     * @param filePath The path to the file
     */
    // Check if the texture is already loaded
    auto tex = textures.find(filePath);
    if (tex != textures.end()) {
        // If the texture is found, return it
        return tex->second;  // t->first is the key and t->second is the value
    }

    // If the texture is not found, load it
    SDL_Texture* newTexture = IMG_LoadTexture(renderer, filePath.c_str());
    if (newTexture == nullptr) {
        std::cout << "Failed to load texture from " << filePath << "! SDL_image Error: " << IMG_GetError() << std::endl;
    } else {
        // If the texture is successfully loaded, store it in the map
        SDL_SetTextureScaleMode(newTexture, SDL_ScaleModeNearest);
        textures[filePath] = newTexture;
    }

    return newTexture;
}

void TextureManager::freeTexture(SDL_Texture* texture) {
    /**
     * Free a texture
     *
     * @param texture The texture to free
     */
    // Find the texture in the map
    for (auto tex = textures.begin(); tex != textures.end(); ++tex) {
        if (tex->second == texture) {
            // If the texture is found, destroy it and remove it from the map
            SDL_DestroyTexture(tex->second);
            textures.erase(tex);
            break;
        }
    }
}