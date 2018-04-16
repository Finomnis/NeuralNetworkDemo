#pragma once

#include "../NeuralNetwork/NeuralNetwork.hxx"
#include "../Util/SDL2Windows.hxx"

#include <memory>

class PointsRegressionDemo : public SDL2Subwindow
{
    protected:
        PointsRegressionDemo() = default;
        void addTrainingSample(double x, double y);
        void createNetwork(std::vector<std::unique_ptr<NeuralNetwork::Layer>> &&layers,
                           std::unique_ptr<NeuralNetwork::ErrorLayer> &&errorLayer);

        void render(SDL_Renderer *renderer, SDL_Rect &rect) override;
        void onEvent(SDL_Event &event, SDL_Rect &subwindowSize) override;

    private:
        std::unique_ptr<NeuralNetwork::Network> network;
};

