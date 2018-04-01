#include "Layer.hxx"

#include "../Util/Log.hxx"

namespace NeuralNetwork
{

Layer::Layer(size_t inputSize, size_t outputSize, size_t numParameters)
    : inputSize(inputSize),
      outputSize(outputSize),
      numParameters(numParameters),
      inputValues(inputSize),
      outputValues(outputSize),
      parameters(numParameters),
      currentGradient(numParameters),
      currentInputGradient(inputSize),
      gradientSum(numParameters, 0),
      numGradients(0)
{}

void Layer::setInputValues(const std::vector<float> &input)
{
    if (input.size() != inputValues.size())
        Log::errAndQuit("Layer input size does not match!");

    inputValues = input;
}

const std::vector<float> &Layer::getOutputValues() const
{
    return outputValues;
}

void Layer::walkGradient(float stepWidth)
{
    if (numGradients == 0)
    {
        Log::err("Unable to walk gradient. No samples previously processed");
        return;
    }

    if (parameters.size() != gradientSum.size())
        Log::errAndQuit("Gradient size doesn't match parameter size!");

    float oneOverNumGradients = 1.0f / float(numGradients);
    for (size_t i = 0; i < parameters.size(); i++)
    {
        parameters[i] -= stepWidth * gradientSum[i] * oneOverNumGradients;
    }
}

void Layer::finishCurrentTrainingSample()
{
    if (currentGradient.size() != gradientSum.size())
        Log::errAndQuit("Gradient and Gradient Sum do not match!");

    for (size_t i = 0; i < currentGradient.size(); i++)
    {
        gradientSum[i] += currentGradient[i];
    }
    numGradients++;
}

void Layer::clearTrainingData()
{
    std::fill(gradientSum.begin(), gradientSum.end(), 0);
    numGradients = 0;
}

void Layer::compute()
{
    if (outputValues.size() != outputSize)
        Log::errAndQuit("Internal error: outputValues array has incorrect size!");

    op(inputValues, outputValues);
}

const std::vector<float> &Layer::backPropagate(const std::vector<float> &childGradient)
{
    if (currentGradient.size() != numParameters)
        Log::errAndQuit("Internal error: currentGradient array has incorrect size!");

    bprop(inputValues, childGradient, currentInputGradient, currentGradient);

    return currentInputGradient;
}

void Layer::computeDirect(const std::vector<float> &input, std::vector<float> &output) const
{
    output.resize(outputSize);

    op(input, output);
}

void Layer::setParameter(size_t id, float value)
{
    parameters[id] = value;
}

float Layer::getParameter(size_t id) const
{
    return parameters[id];
}


}
