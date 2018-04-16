#pragma once

#include "../NeuralNetwork/NeuralNetwork.hxx"
#include "../Util/SDL2Windows.hxx"

#include <memory>
#include <atomic>
#include <tuple>

class Relu : public SDL2Subwindow
{
    public:
        Relu();
        virtual ~Relu() = default;
        void addTrainingSample(double x, double y);
        void run();

        void render(SDL_Renderer *renderer, SDL_Rect &rect) override;
        void onEvent(SDL_Event &event, SDL_Rect &subwindowSize) override;

    private:
        std::unique_ptr<NeuralNetwork::Network> network;
};

