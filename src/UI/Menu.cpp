//
// Created by adam slay on 2/14/24.
//
#include <iostream>

#include "Menu.h"

void start_menu(SDL_Renderer* renderer, TTF_Font* font) {
    /**
     * Set up the start menu for the game
     */

    // Set up the surface for the text
    std::string text = "Bummer Engine";
    SDL_Color color = {255, 245, 140};
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (textSurface == nullptr) {
        std::cout << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return;
    }

    // Set up the texture for the text
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (textTexture == nullptr) {
        std::cout << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << std::endl;
        return;
    }

    // Get the width and height of the text then free the surface
    int textWidth = textSurface->w + 200;
    int textHeight = textSurface->h + 45;
    SDL_FreeSurface(textSurface);

    // Set the position of the text then create a rectangle to render the text to
    int x = 100;
    int y = 140;
    SDL_Rect renderQuad = {x, y, textWidth, textHeight};

    // Copy the texture to the renderer then destroy the texture
    SDL_RenderCopy(renderer, textTexture, nullptr, &renderQuad);
    SDL_DestroyTexture(textTexture);
}