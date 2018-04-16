#pragma once

#include "../NeuralNetwork/NeuralNetwork.hxx"

class ReluLayer : public NeuralNetwork::Layer
{
    public:
        ReluLayer(size_t numLayers);

    private:
        void op(const std::vector<double> &input,
                std::vector<double> &output) const override;
        void bprop(const std::vector<double> &input,
                   const std::vector<double> &outputGradient,
                   std::vector<double> &inputGradient,
                   std::vector<double> &parameterGradient) const override;
};
