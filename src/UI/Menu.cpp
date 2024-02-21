#include <iostream>

#include "../Config.h"
#include "Menu.h"


void render_splash_screen(SDL_Renderer* renderer, TTF_Font* font) {
    /**
     * Render the splash screen on startup
     *
     * @param renderer: The renderer to render the splash screen to
     * @param font: The font to use for the splash screen
     */
    SDL_SetRenderDrawColor(renderer, 252,226,137, 0xFF);  // bb_yellow
    SDL_RenderClear(renderer);
    splash_screen(renderer, font);
    SDL_RenderPresent(renderer);
    SDL_Delay(5000);
}

void splash_screen(SDL_Renderer* renderer, TTF_Font* font) {
    /**
     * Render the splash screen
     *
     * @param renderer: The renderer to render the splash screen to
     * @param font: The font to use for the splash screen
     */
    std::string mainText = "Bummer Engine";
    int textWidth, textHeight;
    TTF_SizeText(font, mainText.c_str(), &textWidth, &textHeight);
    int mainX = (SCREEN_WIDTH / 2) - (textWidth / 2);
    int mainY = (SCREEN_HEIGHT / 2) - (textHeight / 2);
    SDL_Color bb_green = {36, 188, 148};
    render_text(renderer, font, mainText, bb_green, mainX, mainY);

    std::string poweredByText = "powered by";
    TTF_SizeText(font, mainText.c_str(), &textWidth, &textHeight);
    int poweredByX = (SCREEN_WIDTH / 2) - (textWidth / 2) + 50;
    int poweredByY = mainY - FONT_SIZE - 10;
    SDL_Color bb_purple{104, 102, 182, 0xFF};
    render_text(renderer, font, poweredByText, bb_purple, poweredByX, poweredByY);
}

void render_text(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, SDL_Color color, int x, int y, int wrapLength) {
    /**
     * Render text to the screen
     *
     * @param renderer: The renderer to render the text to
     * @param font: The font to use for the text
     * @param text: The text to render
     * @param color: The color of the text
     * @param x: The x position of the text
     * @param y: The y position of the text
     * @param wrapLength: The length to wrap the text at
     */

    // Set up the surface for the text
    SDL_Surface* textSurface = TTF_RenderUTF8_Blended_Wrapped(font, text.c_str(), color, wrapLength);
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
    int textWidth = textSurface->w;
    int textHeight = textSurface->h;
    SDL_FreeSurface(textSurface);

    // Set the position of the text then create a rectangle to render the text to
    SDL_Rect renderQuad = {x, y, textWidth, textHeight};

    // Copy the texture to the renderer then destroy the texture
    SDL_RenderCopy(renderer, textTexture, nullptr, &renderQuad);
    SDL_DestroyTexture(textTexture);
}