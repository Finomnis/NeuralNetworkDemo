#include "SDL2Windows.hxx"
#include "Log.hxx"

#include <sstream>
#include <iostream>

namespace
{

#define SDL(func){\
    if((func)!=0){\
        std::stringstream ss;\
        ss << __FILE__ << "(" << __LINE__ << "): SDL: " << SDL_GetError();\
        Log::errAndQuit(ss.str());\
    }\
}

}

SDL2Initializer::SDL2Initializer()
{
    Log::msg("Initializing SDL2 ...");
    SDL_SetMainReady();
    SDL(SDL_Init(SDL_INIT_EVERYTHING));
}

SDL2Initializer::~SDL2Initializer()
{
    Log::msg("Shutting down SDL2 ...");
    SDL_Quit();
}

void SDL2Initializer::runMainLoop()
{
    bool loop = true;
    while (loop)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    loop = false;
                    break;
                case SDL_WINDOWEVENT:
                    for (auto &window : windows)
                    {
                        if (event.window.windowID == SDL_GetWindowID(window->window))
                            window->onEvent(event);
                    }
                    break;
                default:
                    for (auto &window : windows)
                    {
                        window->onEvent(event);
                    }
                    break;
            }
        }

        for (auto &window : windows)
        {
            window->render();
        }
    }
}


SDL2Window::SDL2Window(std::shared_ptr<SDL2Initializer> init_, std::string name)
    : init(init_)
{
    Log::msg("Creating window '" + name + "' ...");
    window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              640, 480,
                              SDL_WINDOW_MAXIMIZED | SDL_WINDOW_RESIZABLE);
    if (window == nullptr)
        Log::errAndQuit(SDL_GetError());

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr)
        Log::errAndQuit(SDL_GetError());

    Log::msg("Adding window '" + name  + "' to initializer ...");
    init->windows.insert(this);
}

SDL2Window::~SDL2Window()
{
    std::string name(SDL_GetWindowTitle(window));
    Log::msg("Removing window '" + name  + "' from initializer ...");
    init->windows.erase(this);
    Log::msg("Destroying window '" + name + "' ...");
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void
SDL2Window::onEvent(SDL_Event &event)
{
    for (auto &subwindow : subwindows)
    {
        SDL_Rect rect = getSubwindowRect(subwindow.first);
        subwindow.first->onEvent(event, rect);
    }
}

void
SDL2Window::render()
{
    SDL_RenderSetClipRect(renderer, nullptr);
    SDL(SDL_SetRenderDrawColor(renderer, 64, 0, 128, 255));
    SDL(SDL_RenderClear(renderer));

    for (auto &subwindow : subwindows)
    {
        SDL_Rect rect = getSubwindowRect(subwindow.second);
        SDL_RenderSetClipRect(renderer, &rect);
        subwindow.first->render(renderer, rect);
    }

    SDL_RenderPresent(renderer);
}

void SDL2Window::addSubwindow(SDL2Subwindow *subwindow, float x0, float y0, float x1, float y1)
{
    subwindows[subwindow] = SDL2SubwindowSize(x0, y0, x1, y1);
}

void SDL2Window::removeSubwindow(SDL2Subwindow *subwindow)
{
    subwindows.erase(subwindow);
}

SDL_Rect SDL2Window::getSubwindowRect(SDL2SubwindowSize &size)
{
    int w, h;
    SDL(SDL_GetRendererOutputSize(renderer, &w, &h));
    int x0 = int(size.x0 * w + 0.5f);
    int x1 = int(size.x1 * w + 0.5f);
    int y0 = int(size.y0 * h + 0.5f);
    int y1 = int(size.y1 * h + 0.5f);
    SDL_Rect rect;
    rect.x = x0;
    rect.y = y0;
    rect.w = x1 - x0;
    rect.h = y1 - y0;
    return rect;
}
SDL_Rect SDL2Window::getSubwindowRect(SDL2Subwindow *window)
{
    return getSubwindowRect(subwindows.at(window));
}


SDL2Subwindow::SDL2Subwindow()
{

}

SDL2Subwindow::~SDL2Subwindow()
{
    if (window)
        window->removeSubwindow(this);
}

void SDL2Subwindow::addToWindow(std::shared_ptr<SDL2Window> window_, float x0, float y0, float x1, float y1)
{
    window = window_;
    window->addSubwindow(this, x0, y0, x1, y1);
}

SDL_Point SDL2Subwindow::relativeMousePosition(int x, int y, SDL_Rect &subwindowSize)
{
    return SDL_Point({x - subwindowSize.x, y - subwindowSize.y});
}

bool SDL2Subwindow::isMouseInsideSubwindow(int x, int y, SDL_Rect &subwindowSize)
{
    if (x < subwindowSize.x || x >= subwindowSize.x + subwindowSize.w)
        return false;
    if (y < subwindowSize.y || y >= subwindowSize.y + subwindowSize.h)
        return false;
    return true;
}
