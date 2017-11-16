#pragma once

#include <list>
#include <vector>

#include "../Log.hxx"

namespace NeuralNetwork
{

struct TrainingSample
{
    std::vector<float> inputs;
    std::vector<float> outputs;
};

class NeuralNetwork
{
    public:
        NeuralNetwork(int inputSize, int outputSize);
        ~NeuralNetwork();
        template< class It >
        void setTrainingData(It begin, It end);
        inline int getNumLayers();
        inline int getLayerSize(int layer);
        inline float getWeight(int outputLayer, int inputIndex, int outputIndex);
        inline float getBias(int layer, int index);
        inline int getInputSize();
        inline int getOutputSize();
        std::vector<float> run(std::vector<float> input);
        void reinitialize(std::vector<int> config);
        void doTrainingStep();

    private:
        std::vector<std::vector<std::vector<float>>> weights;
        std::vector<std::vector<float>> biases;
        std::vector<TrainingSample> trainingData;
        size_t maxLayerSize;
};

template< class It >
void NeuralNetwork::setTrainingData(It begin, It end)
{
    trainingData.assign(begin, end);
    Log::msg("Got " + std::to_string(trainingData.size()) + " samples.");
}

inline float NeuralNetwork::getWeight(int outputLayer,  int inputIndex, int outputIndex)
{
    return weights.at(outputLayer - 1).at(outputIndex).at(inputIndex);
}

inline float NeuralNetwork::getBias(int layer, int index)
{
    return biases.at(layer).at(index);
}

inline int NeuralNetwork::getInputSize()
{
    return getLayerSize(0);
}

inline int NeuralNetwork::getOutputSize()
{
    return getLayerSize(getNumLayers() - 1);
}

inline int NeuralNetwork::getNumLayers()
{
    return biases.size();
}

inline int NeuralNetwork::getLayerSize(int layer)
{
    return biases.at(layer).size();
}


}
