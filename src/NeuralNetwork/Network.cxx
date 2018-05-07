#include "Network.hxx"


namespace NeuralNetwork
{


Network::Network(std::vector<std::unique_ptr<Layer> > &&layers_,
                 std::unique_ptr<ErrorLayer> &&errorLayer_)
    : layers(std::move(layers_)),
      errorLayer(std::move(errorLayer_))
{

}

const std::vector<double> Network::compute(const std::vector<double> &input)
{
    if (layers.empty())
        return input;

    std::vector<double> currentInput = input;
    std::vector<double> currentOutput;
    for (const auto &layer : layers)
    {
        currentOutput.resize(layer->outputSize);
        layer->computeDirect(currentInput, currentOutput);
        std::swap(currentInput, currentOutput);
    }

    return currentInput;
}

void Network::addTrainingSample(const std::vector<double> &input,
                                const std::vector<double> &output)
{
    trainingInputs.push_back(input);
    trainingOutputs.push_back(output);
}

size_t Network::getNumLayers() const
{
    return layers.size();
}

size_t Network::getNumParameters(size_t layerId) const
{
    return layers.at(layerId)->getNumParameters();
}

size_t Network::getPreferredParameterGrouping(size_t layerId) const
{
    const auto &layer = layers.at(layerId);

    if (layer->outputSize == 0)
        return 0;

    return layer->getNumParameters() / layer->outputSize;
}

double Network::trainingStep(double stepWidth)
{
    double error = 0;

    for (auto &layer : layers)
        layer->clearTrainingData();

    for (size_t i = 0; i < trainingInputs.size(); i++)
    {
        // forward
        {
            std::vector<double> inputValues = trainingInputs.at(i);
            for (auto &layer : layers)
            {
                layer->setInputValues(inputValues);
                layer->compute();
                inputValues = layer->getOutputValues();
            }
            errorLayer->setExpectedResult(trainingOutputs.at(i));
            errorLayer->setInputValues(inputValues);
            errorLayer->compute();
            error += errorLayer->getOutputValues().at(0);
        }

        // back propagation
        {
            std::vector<double> childGradient = errorLayer->backPropagate(std::vector<double>({1}));
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

double Network::getParameter(size_t layerID, size_t parameterID) const
{
    return layers.at(layerID)->getParameter(parameterID);
}

const std::vector<double> &Network::getTrainingInput(size_t id) const
{
    return trainingInputs.at(id);
}
const std::vector<double> &Network::getTrainingOutput(size_t id) const
{
    return trainingOutputs.at(id);
}

double Network::getNumTrainingSamples() const
{
    return trainingInputs.size();
}

}
