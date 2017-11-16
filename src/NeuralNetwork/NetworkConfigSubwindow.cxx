#include "NetworkConfigSubwindow.hxx"

#include "NeuralNetwork.hxx"

#include <iostream>
#include <ios>

namespace NeuralNetwork
{
NetworkConfigSubwindow::NetworkConfigSubwindow(std::shared_ptr<NeuralNetwork> nn)
    : neuralNetwork(nn)
{

}

NetworkConfigSubwindow::~NetworkConfigSubwindow()
{

}

void NetworkConfigSubwindow::render(SDL_Renderer *renderer, SDL_Rect &rect)
{
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderFillRect(renderer, &rect);
}

void NetworkConfigSubwindow::onEvent(SDL_Event &event, SDL_Rect &subwindowSize)
{
    //SDL_Point mousePos;
    //bool leftButton = false;
    switch (event.type)
    {
        case SDL_MOUSEBUTTONDOWN:
            if (!isMouseInsideSubwindow(event.button.x, event.button.y, subwindowSize))
                return;
            //mousePos = relativeMousePosition(event.button.x, event.button.y, subwindowSize);
            //leftButton = (event.button.button == SDL_BUTTON_LEFT);
            break;
        case SDL_MOUSEBUTTONUP:
            return;
        case SDL_KEYDOWN:
            return;
        default:
            return;
    }

}


}
