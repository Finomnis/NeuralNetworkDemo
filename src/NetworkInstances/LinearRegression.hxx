#pragma once

#include "../NeuralNetwork/NeuralNetwork.hxx"

#include <memory>

class LinearRegression
{
    public:
        LinearRegression();
        void addTrainingSample(float x, float y);
        void run();

    private:
        std::unique_ptr<NeuralNetwork::Network> network;
};
