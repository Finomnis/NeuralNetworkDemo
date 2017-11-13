#pragma once

#include <list>
#include <vector>

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
        void setTrainingData(std::list<TrainingSample> trainingSamples);
};

}
