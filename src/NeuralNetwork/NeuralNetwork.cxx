#include "NeuralNetwork.hxx"

#include "../Log.hxx"

#include <random>
#include <chrono>
#include <algorithm>

#include <iostream>

namespace NeuralNetwork
{

NeuralNetwork::NeuralNetwork(int inputSize, int outputSize)
{
    reinitialize({inputSize, 3, 3, 3, 3, 3, outputSize});
}

NeuralNetwork::~NeuralNetwork()
{}


void NeuralNetwork::reinitialize(std::vector<int> config)
{
    std::default_random_engine randomDevice(time(0));

    // Weights
    {
        std::uniform_real_distribution<float> floatRand(-10.0f, 10.0f);
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
    }

    // Biases
    {
        std::uniform_real_distribution<float> floatRand(-0.2f, 0.2f);

        biases.resize(config.size());
        for (size_t i = 0; i < config.size(); i++)
        {
            biases[i].resize(config[i]);
            for (int j = 0; j < config[i]; j++)
            {
                biases[i][j] = floatRand(randomDevice);
                if (i == 0) biases[i][j] += 0.5f;
            }
        }
    }

    // maxLayerSize, for performance optimization
    maxLayerSize = *(std::max_element(config.begin(), config.end()));
}

std::vector<float>
NeuralNetwork::run(std::vector<float> input)
{
    std::vector<float> currentValues;
    currentValues.resize(maxLayerSize);
    std::vector<float> previousValues;
    previousValues.resize(maxLayerSize);

    for (size_t i = 0; i < input.size(); i++)
    {
        currentValues[i] = input[i] - getBias(0, i);
    }

    for (int layer = 1; layer < getNumLayers(); layer++)
    {
        std::swap(currentValues, previousValues);
        for (int neuron = 0; neuron < getLayerSize(layer); neuron++)
        {
            float value = -getBias(layer, neuron);
            for (int inputNeuron = 0; inputNeuron < getLayerSize(layer - 1); inputNeuron++)
            {
                value += previousValues[inputNeuron] * getWeight(layer, inputNeuron, neuron);
            }
            if (layer != getNumLayers() - 1)
            {
                value = value / (1 + std::fabs(value));
            }
            currentValues[neuron] = value;
        }
    }

    currentValues.resize(getOutputSize());

    return currentValues;
}


}
