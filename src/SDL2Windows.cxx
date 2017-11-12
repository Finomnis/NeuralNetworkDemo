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

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
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
    // TODO
}

void
SDL2Window::render()
{
    SDL_SetRenderDrawColor(renderer, 64, 0, 128, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    // TODO render subwindows
}

SDL2Subwindow::SDL2Subwindow()
{

}

SDL2Subwindow::~SDL2Subwindow()
{

}

void SDL2Subwindow::addToWindow(std::shared_ptr<SDL2Window>, float x0, float y0, float x1, float y1)
{
    // TODO
}
