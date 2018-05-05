#include "SigmoidLayer.hxx"

#include <cmath>

SigmoidLayer::SigmoidLayer(size_t numLayers): NeuralNetwork::Layer(numLayers, numLayers, 0)
{
}

namespace
{
double sigmoid(double x)
{
    return 1.0 / (1.0 + std::exp(-x));
}
}

void SigmoidLayer::op(const std::vector<double> &input,
                      std::vector<double> &output) const
{
    for (size_t i = 0; i < inputSize; i++)
    {
        double x = input.at(i);

        output.at(i) = sigmoid(x);
    }
}

void SigmoidLayer::bprop(const std::vector<double> &input,
                         const std::vector<double> &outputGradient,
                         std::vector<double> &inputGradient,
                         std::vector<double> &) const
{
    for (size_t i = 0; i < inputSize; i++)
    {
        double x = input.at(i);

        double sigX = sigmoid(x);

        inputGradient.at(i) = sigX * (1.0 - sigX) * outputGradient.at(i);
    }
}
