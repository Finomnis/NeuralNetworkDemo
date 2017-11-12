#include "InteractiveSubwindow.hxx"

#include <iostream>
#include <ios>

namespace NeuralNetwork
{
InteractiveSubwindow::InteractiveSubwindow(std::shared_ptr<NeuralNetwork> nn)
    : neuralNetwork(nn)
{

}

InteractiveSubwindow::~InteractiveSubwindow()
{

}

SDL_Point mousePos;
bool mouseEnabled = false;

void InteractiveSubwindow::render(SDL_Surface *surface)
{
    Uint32 col = SDL_MapRGB(surface->format, 128, 128, 0);
    SDL_FillRect(surface, &(surface->clip_rect), col);

    if (mouseEnabled)
    {
        Uint32 col2 = SDL_MapRGB(surface->format, 255, 0, 0);
        SDL_Rect rect = {mousePos.x - 5, mousePos.y - 5, 11, 11};
        SDL_FillRect(surface, &rect, col2);
    }
    else
    {
        Uint32 col2 = SDL_MapRGB(surface->format, 0, 255, 0);
        SDL_Rect rect = {mousePos.x - 5, mousePos.y - 5, 11, 11};
        SDL_FillRect(surface, &rect, col2);
    }
}

void InteractiveSubwindow::onEvent(SDL_Event &event, SDL_Rect &subwindowSize)
{
    switch (event.type)
    {
        case SDL_MOUSEMOTION:
            mousePos = relativeMousePosition(event.motion.x, event.motion.y, subwindowSize);
            break;
        case SDL_MOUSEBUTTONDOWN:
            mouseEnabled = true;
            break;
        case SDL_MOUSEBUTTONUP:
            mouseEnabled = false;
            break;
    }
}

}
