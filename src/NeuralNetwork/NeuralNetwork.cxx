#include "NeuralNetwork.hxx"

#include "../Log.hxx"

#include <random>
#include <chrono>

namespace NeuralNetwork
{

NeuralNetwork::NeuralNetwork(int inputSize, int outputSize)
{
    reinitialize({inputSize, 1, outputSize});
}

NeuralNetwork::~NeuralNetwork()
{}

inline int NeuralNetwork::getNumLayers()
{
    return weights.size();
}

inline int NeuralNetwork::getLayerSize(int layer)
{
    return weights.at(layer).size();
}

inline float NeuralNetwork::getWeight(int layer, int index, int inputIndex)
{
    return weights.at(layer).at(index).at(inputIndex);
}

inline float NeuralNetwork::getBias(int layer, int index)
{
    return biases.at(layer).at(index);
}

void NeuralNetwork::reinitialize(std::vector<int> config)
{
    std::default_random_engine randomDevice(0);//std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()).count());
    std::uniform_real_distribution<float> floatRand(-1.0f, 1.0f);

    // Weights
    weights.resize(config.size() - 1);
    for (size_t i = 0; i < config.size() - 1; i++)
    {
        weights[i].resize(config[i + 1]);
        for (int j = 0; j < config[i + 1]; j++)
        {
            weights[i][j].resize(config[i]);
            for (int k = 0; k < config[i]; k++)
            {
                weights[i][j][k] = floatRand(randomDevice);
            }
        }
    }

    // Biases
    biases.resize(config.size());
    for (size_t i = 0; i < config.size(); i++)
    {
        biases[i].resize(config[i]);
        for (int j = 0; j < config[i]; j++)
        {
            biases[i][j] = floatRand(randomDevice);
        }
    }
}

}
