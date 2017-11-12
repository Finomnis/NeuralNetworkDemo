#pragma once

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include <memory>
#include <set>
#include <map>

class SDL2Window;
class SDL2Initializer
{
        friend class SDL2Window;
    public:
        SDL2Initializer();
        ~SDL2Initializer();
        std::shared_ptr<SDL2Window> createWindow(std::string name);
        void runMainLoop();

    private:
        std::set<SDL2Window *> windows;
};

struct SDL2SubwindowSize
{
    SDL2SubwindowSize()
        : x0(0), y0(0), x1(1), y1(1)
    {}
    SDL2SubwindowSize(float x0, float y0, float x1, float y1)
        : x0(x0), y0(y0), x1(x1), y1(y1)
    {}
    float x0, y0, x1, y1;
};

class SDL2Subwindow;
class SDL2Window
{
        friend class SDL2Initializer;
        friend class SDL2Subwindow;
    public:
        SDL2Window(std::shared_ptr<SDL2Initializer> init, std::string name);
        ~SDL2Window();

    private:
        SDL_Window *window;
        SDL_Renderer *renderer;
        std::shared_ptr<SDL2Initializer> init;
        std::map<SDL2Subwindow *, SDL2SubwindowSize> subwindows;

    private:
        void onEvent(SDL_Event &event);
        void render();

};

class SDL2Subwindow
{
    public:
        SDL2Subwindow();
        ~SDL2Subwindow();
        void addToWindow(std::shared_ptr<SDL2Window> window, float x0, float y0, float x1, float y1);
        virtual void render(SDL_Renderer *renderer, SDL_Rect &rect) = 0;
        virtual void onEvent(SDL_Event &event) = 0;

    private:
        std::shared_ptr<SDL2Window> window;
};


