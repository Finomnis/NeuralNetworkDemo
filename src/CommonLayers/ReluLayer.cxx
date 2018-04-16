#include "ReluLayer.hxx"

ReluLayer::ReluLayer(size_t numLayers): NeuralNetwork::Layer(numLayers, numLayers, 0)
{
}

void ReluLayer::op(const std::vector<float> &input,
                   std::vector<float> &output) const
{
    for (size_t i = 0; i < input.size(); i++)
    {
        float x = input.at(i);

        if (x > 0)
            output.at(i) = x;
        else
            output.at(i) = 0;
    }
}

void ReluLayer::bprop(const std::vector<float> &input,
                      const std::vector<float> &outputGradient,
                      std::vector<float> &inputGradient,
                      std::vector<float> &) const
{
    for (size_t i = 0; i < input.size(); i++)
    {
        float x = input.at(i);

        if (x > 0)
            inputGradient.at(i) = outputGradient.at(i);
        else
            inputGradient.at(i) = 0;
    }
}
