#include "SDL2Windows.hxx"
#include "Log.hxx"

SDL2Initializer::SDL2Initializer()
{
    Log::msg("Initializing SDL2 ...");
    SDL_SetMainReady();
    SDL_Init(SDL_INIT_EVERYTHING);
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
        subwindow.first->onEvent(event);
    }
}

void
SDL2Window::render()
{
    SDL_SetRenderDrawColor(renderer, 64, 0, 128, 255);
    SDL_RenderClear(renderer);

    int w, h;
    SDL_GetRendererOutputSize(renderer, &w, &h);
    for (auto &subwindow : subwindows)
    {
        const SDL2SubwindowSize &size = subwindow.second;
        int x0 = int(size.x0 * w + 0.5f);
        int x1 = int(size.x1 * w + 0.5f);
        int y0 = int(size.y0 * h + 0.5f);
        int y1 = int(size.y1 * h + 0.5f);
        SDL_Rect rect;
        rect.x = x0;
        rect.y = y0;
        rect.w = x1 - x0;
        rect.h = y1 - y0;
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
    {
        auto renderTarget = subwindowRenderTargets.find(subwindow);
        if (renderTarget != subwindowRenderTargets.end())
        {
            SDL_FreeSurface(renderTarget->second);
        }
    }
    subwindowRenderTargets.erase(subwindow);
    subwindows.erase(subwindow);
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

