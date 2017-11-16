#pragma once

#include "../SDL2Windows.hxx"

#include <memory>
#include <list>
#include <vector>

namespace NeuralNetwork
{

class NeuralNetwork;
struct TrainingSample;

class NetworkConfigSubwindow : public SDL2Subwindow
{
    public:
        NetworkConfigSubwindow(std::shared_ptr<NeuralNetwork> nn);
        ~NetworkConfigSubwindow();

        void render(SDL_Renderer *renderer, SDL_Rect &rect);
        void onEvent(SDL_Event &event, SDL_Rect &subwindowSize);


    private:
        std::shared_ptr<NeuralNetwork> neuralNetwork;


};

}
