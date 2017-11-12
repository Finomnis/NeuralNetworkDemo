#pragma once

#include "../SDL2Windows.hxx"

#include <memory>

namespace NeuralNetwork
{

class NeuralNetwork;

class InteractiveSubwindow : public SDL2Subwindow
{
    public:
        InteractiveSubwindow(std::shared_ptr<NeuralNetwork> nn);
        ~InteractiveSubwindow();

        void render(SDL_Renderer *renderer, SDL_Rect &rect);
        void onEvent(SDL_Event &event);


    private:
        std::shared_ptr<NeuralNetwork> neuralNetwork;
};

}
