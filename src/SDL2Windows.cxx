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
