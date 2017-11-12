#include "SDL2Windows.hxx"

#include "Log.hxx"

int main(int argc, char *argv[])
{
    SDL2Initializer sdl2Initializer;

    Log::msg("Test");

    return 0;
}
