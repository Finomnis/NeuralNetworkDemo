#pragma once

#include "../NeuralNetwork/NeuralNetwork.hxx"

class MeanSquaredErrorLayer : public NeuralNetwork::ErrorLayer
{
    public:
        MeanSquaredErrorLayer(size_t size);

    private:
        void op(const std::vector<double> &input,
                std::vector<double> &output) const override;
        void bprop(const std::vector<double> &input,
                   const std::vector<double> &outputGradient,
                   std::vector<double> &inputGradient,
                   std::vector<double> &parameterGradient) const override;
};
