#include "InteractiveSubwindow.hxx"

namespace NeuralNetwork
{
InteractiveSubwindow::InteractiveSubwindow(std::shared_ptr<NeuralNetwork> nn)
    : neuralNetwork(nn)
{

}

InteractiveSubwindow::~InteractiveSubwindow()
{

}


void InteractiveSubwindow::render(SDL_Renderer *renderer, SDL_Rect &rect)
{
    SDL_SetRenderDrawColor(renderer, 128, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}

void InteractiveSubwindow::onEvent(SDL_Event &event)
{

}

}
