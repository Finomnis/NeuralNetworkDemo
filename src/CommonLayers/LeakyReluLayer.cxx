#include "LeakyReluLayer.hxx"

LeakyReluLayer::LeakyReluLayer(size_t numLayers): NeuralNetwork::Layer(numLayers, numLayers, 0)
{
}

void LeakyReluLayer::op(const std::vector<double> &input,
                        std::vector<double> &output) const
{
    for (size_t i = 0; i < inputSize; i++)
    {
        double x = input.at(i);

        if (x > 0)
            output.at(i) = x;
        else
            output.at(i) = 0.01 * x;
    }
}

void LeakyReluLayer::bprop(const std::vector<double> &input,
                           const std::vector<double> &outputGradient,
                           std::vector<double> &inputGradient,
                           std::vector<double> &) const
{
    for (size_t i = 0; i < inputSize; i++)
    {
        double x = input.at(i);

        if (x > 0)
            inputGradient.at(i) = outputGradient.at(i);
        else
            inputGradient.at(i) = 0.01 * outputGradient.at(i);
    }
}
