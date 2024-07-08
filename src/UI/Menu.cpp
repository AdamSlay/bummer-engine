#include <iostream>

#include "Menu.h"
#include "../Config.h"

Menu::Menu(SDL_Renderer* renderer, TTF_Font* font) : renderer(renderer), font(font) {};


void Menu::update(bool& startMenu, bool& quit) {
    /**
     * Show the start menu and handle user input.
     * If the user presses the play button, start the game.
     * If the user presses the exit button, quit the game.
     *
     * @param startMenu: A boolean to determine if the start menu should be shown
     * @param quit: A boolean to determine if the game should quit
     */
    Menu::MenuResult result = Show();
    if(result == Menu::Exit) {
        quit = true;
    }
    if(result == Menu::Play) {
        startMenu = false;
    }
}

Menu::MenuResult Menu::Show() {
    /**
     * Show the start menu and handle user input.
     *
     */
    SDL_Event menuEvent;

    while(true) {
        if(SDL_PollEvent(&menuEvent)) {
            if(menuEvent.type == SDL_QUIT) return Exit;
            if(menuEvent.type == SDL_KEYDOWN) {
                switch(menuEvent.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        return Exit;
                    case SDLK_RETURN:
                        return Play;
                    default:
                        break;
                }
            }
        }

        render_menu();
    }
}


void Menu::render_menu() {
    /**
     * Render the splash screen on startup
     */
    SDL_SetRenderDrawColor(renderer, 252,226,137, 0xFF);  // bb_yellow
    SDL_RenderClear(renderer);
    menu_view();
    SDL_RenderPresent(renderer);
}

void Menu::menu_view() {
    /**
     * Render the splash screen
     *
     */
    std::string mainText = "Press ENTER to Play";
    int textWidth, textHeight;
    TTF_SizeText(font, mainText.c_str(), &textWidth, &textHeight);
    int mainX = (SCREEN_WIDTH / 2) - (textWidth / 2);
    int mainY = (SCREEN_HEIGHT / 2) - (textHeight / 2);
    SDL_Color bb_green = {36, 188, 148};
    render_text(mainText, bb_green, mainX, mainY);

    std::string poweredByText = "Press ESCAPE to Quit";
    TTF_SizeText(font, mainText.c_str(), &textWidth, &textHeight);
    int poweredByX = (SCREEN_WIDTH / 2) - (textWidth / 2);
    int poweredByY = mainY - FONT_SIZE - 50;
    SDL_Color bb_purple{104, 102, 182, 0xFF};
    render_text(poweredByText, bb_purple, poweredByX, poweredByY);
}

void Menu::render_text(const std::string& text, SDL_Color color, int x, int y, int wrapLength) {
    /**
     * Render text to the screen
     *
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
