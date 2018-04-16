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
        SDL2Window(std::shared_ptr<SDL2Initializer> init, const std::string &name);
        ~SDL2Window();

    private:
        SDL_Window *window;
        SDL_Renderer *renderer;
        std::shared_ptr<SDL2Initializer> init;
        std::map<SDL2Subwindow *, SDL2SubwindowSize> subwindows;

        void addSubwindow(SDL2Subwindow *, float x0, float y0, float x1, float y1);
        void removeSubwindow(SDL2Subwindow *);

    private:
        void onEvent(SDL_Event &event);
        void render();

    private:
        SDL_Rect getSubwindowRect(SDL2SubwindowSize &size);
        SDL_Rect getSubwindowRect(SDL2Subwindow *window);

};

class SDL2Subwindow
{
    public:
        SDL2Subwindow();
        virtual ~SDL2Subwindow();
        void addToWindow(std::shared_ptr<SDL2Window> window, float x0, float y0, float x1, float y1);
        virtual void render(SDL_Renderer *renderer, SDL_Rect &rect) = 0;
        virtual void onEvent(SDL_Event &event, SDL_Rect &subwindowSize) = 0;

    protected:
        static SDL_Point relativeMousePosition(int x, int y, SDL_Rect &subwindowSize);
        static bool isMouseInsideSubwindow(int x, int y, SDL_Rect &subwindowSize);

    private:
        std::shared_ptr<SDL2Window> window;
};


