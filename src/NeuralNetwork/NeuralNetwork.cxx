#include "NeuralNetwork.hxx"

#include "../Log.hxx"

namespace NeuralNetwork
{


void NeuralNetwork::setTrainingData(std::list<TrainingSample> trainingSamples)
{
    Log::msg("Got " + std::to_string(trainingSamples.size()) + " samples.");
}

}
