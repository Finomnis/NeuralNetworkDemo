#pragma once

#include "../SDL2Windows.hxx"

#include <memory>
#include <list>
#include <vector>

namespace NeuralNetwork
{

class NeuralNetwork;
struct TrainingSample;

class InteractiveSubwindow : public SDL2Subwindow
{
    public:
        InteractiveSubwindow(std::shared_ptr<NeuralNetwork> nn);
        ~InteractiveSubwindow();

        void render(SDL_Renderer *renderer, SDL_Rect &rect);
        void onEvent(SDL_Event &event, SDL_Rect &subwindowSize);


    private:
        std::shared_ptr<NeuralNetwork> neuralNetwork;

        std::list<TrainingSample> samples;
        void removeIntersectingSamples(SDL_Rect &subwindowSize, SDL_Point mousePos);
        void addSample(SDL_Rect &subwindowSize, SDL_Point mousePos, float value);

        bool trainingDataChanged = false;

};

}
