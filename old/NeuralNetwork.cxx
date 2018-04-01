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
    resize({inputSize, 3, 3, 3, 3, 3, outputSize});
    initializeRandom();
}

NeuralNetwork::~NeuralNetwork()
{}


void NeuralNetwork::resize(std::vector<int> config)
{
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
                weights[i][j][k] = 0;
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
            biases[i][j] = 0;
        }
    }

    // maxLayerSize, for performance optimization
    maxLayerSize = *(std::max_element(config.begin(), config.end()));
}

void NeuralNetwork::initializeRandom()
{
    unsigned long milliseconds_since_epoch =
        std::chrono::duration_cast<std::chrono::milliseconds>
        (std::chrono::system_clock::now().time_since_epoch()).count();

    std::mt19937 randomDevice(milliseconds_since_epoch);

    // Weights
    {
        std::uniform_real_distribution<float> floatRand(-10.0f, 10.0f);
        for (size_t i = 0; i < weights.size(); i++)
        {
            for (size_t j = 0; j < weights[i].size(); j++)
            {
                for (size_t k = 0; k < weights[i][j].size(); k++)
                {
                    weights[i][j][k] = floatRand(randomDevice);
                }
            }
        }
    }

    // Biases
    {
        std::uniform_real_distribution<float> floatRand(-0.2f, 0.2f);

        for (size_t i = 0; i < biases.size(); i++)
        {
            for (size_t j = 0; j < biases[i].size(); j++)
            {
                biases[i][j] = floatRand(randomDevice);
                if (i == 0) biases[i][j] += 0.5f;
            }
        }
    }

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

void NeuralNetwork::doTrainingStep()
{
    initializeRandom();
}

}
