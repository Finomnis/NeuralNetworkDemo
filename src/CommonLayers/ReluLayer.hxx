#pragma once

#include "../NeuralNetwork/NeuralNetwork.hxx"

class ReluLayer : public NeuralNetwork::Layer
{
    public:
        ReluLayer(size_t numLayers);

    private:
        void op(const std::vector<float> &input,
                std::vector<float> &output) const override;
        void bprop(const std::vector<float> &input,
                   const std::vector<float> &outputGradient,
                   std::vector<float> &inputGradient,
                   std::vector<float> &) const override;
};
