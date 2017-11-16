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
        int getNumLayers();
        int getLayerSize(int layer);
        float getWeight(int layer, int index, int inputIndex);
        float getBias(int layer, int index);
        int getInputSize();
        int getOutputSize();
        std::vector<float> run(std::vector<float> input);
        void reinitialize(std::vector<int> config);
        void doTrainingStep();

    private:
        std::vector<std::vector<std::vector<float>>> weights;
        std::vector<std::vector<float>> biases;
        std::vector<TrainingSample> trainingData;
};

template< class It >
void NeuralNetwork::setTrainingData(It begin, It end)
{
    trainingData.assign(begin, end);
    Log::msg("Got " + std::to_string(trainingData.size()) + " samples.");
}


}
