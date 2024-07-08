#ifndef BUMMERENGINE_MENU_H
#define BUMMERENGINE_MENU_H

#include <SDL.h>
#include <SDl2/SDL_ttf.h>

class Menu {
public:
    enum MenuResult { Nothing, Exit, Play };

    struct MenuItem {
        SDL_Rect rect;
        MenuResult action;
    };
    Menu(SDL_Renderer* renderer, TTF_Font* font);
    void update(bool& startMenu, bool& quit);
    MenuResult Show();
    void render_menu();
    void menu_view();
    void render_text(const std::string& text, SDL_Color color, int x, int y, int wrapLength = 0);


private:
    // Add private member variables and methods here if needed
    SDL_Renderer* renderer;
    TTF_Font* font;
};

#endif //BUMMERENGINE_MENU_H