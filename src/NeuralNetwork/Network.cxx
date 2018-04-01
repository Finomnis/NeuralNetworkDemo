#include "Network.hxx"


namespace NeuralNetwork
{


Network::Network(std::vector<std::unique_ptr<Layer> > &&layers_,
                 std::unique_ptr<ErrorLayer> &&errorLayer_)
    : layers(std::move(layers_)),
      errorLayer(std::move(errorLayer_))
{

}

const std::vector<float> Network::compute(const std::vector<float> &input)
{
    if (layers.empty())
        return input;

    std::vector<float> currentInput = input;
    std::vector<float> currentOutput;
    for (const auto &layer : layers)
    {
        currentOutput.resize(layer->outputSize);
        layer->computeDirect(currentInput, currentOutput);
        std::swap(currentInput, currentOutput);
    }

    return currentInput;
}

void Network::addTrainingSample(const std::vector<float> &input,
                                const std::vector<float> &output)
{
    trainingInputs.push_back(input);
    trainingOutputs.push_back(output);
}

float Network::trainingStep(float stepWidth)
{
    float error = 0;

    for (auto &layer : layers)
        layer->clearTrainingData();

    for (size_t i = 0; i < trainingInputs.size(); i++)
    {
        // forward
        {
            std::vector<float> inputValues = trainingInputs[i];
            for (auto &layer : layers)
            {
                layer->setInputValues(inputValues);
                layer->compute();
                inputValues = layer->getOutputValues();
            }
            errorLayer->setExpectedResult(trainingOutputs[i]);
            errorLayer->setInputValues(inputValues);
            errorLayer->compute();
            error += errorLayer->getOutputValues()[0];
        }

        // back propagation
        {
            std::vector<float> childGradient = errorLayer->backPropagate(std::vector<float>({1}));
            for (auto it = layers.rbegin(); it != layers.rend(); it++)
            {
                childGradient = (*it)->backPropagate(childGradient);
            }
        }

        for (auto &layer : layers)
        {
            layer->finishCurrentTrainingSample();
        }
    }

    for (auto &layer : layers)
    {
        layer->walkGradient(stepWidth);
    }

    return error;
}

float Network::getParameter(size_t layerID, size_t parameterID) const
{
    return layers.at(layerID)->getParameter(parameterID);
}

const std::vector<float> &Network::getTrainingInput(size_t id) const
{
    return trainingInputs[id];
}
const std::vector<float> &Network::getTrainingOutput(size_t id) const
{
    return trainingOutputs[id];
}

float Network::getNumTrainingSamples() const
{
    return trainingInputs.size();
}

}
