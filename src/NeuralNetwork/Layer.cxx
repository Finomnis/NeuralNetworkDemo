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

void Layer::setInputValues(const std::vector<double> &input)
{
    if (input.size() != inputValues.size())
        Log::errAndQuit("Layer input size does not match!");

    inputValues = input;
}

const std::vector<double> &Layer::getOutputValues() const
{
    return outputValues;
}

void Layer::walkGradient(double stepWidth)
{
    if (numGradients == 0)
    {
        Log::err("Unable to walk gradient. No samples previously processed");
        return;
    }

    if (parameters.size() != gradientSum.size())
        Log::errAndQuit("Gradient size doesn't match parameter size!");

    double oneOverNumGradients = 1.0 / double(numGradients);
    for (size_t i = 0; i < parameters.size(); i++)
    {
        parameters.at(i) -= stepWidth * gradientSum.at(i) * oneOverNumGradients;
    }
}

void Layer::finishCurrentTrainingSample()
{
    if (currentGradient.size() != gradientSum.size())
        Log::errAndQuit("Gradient and Gradient Sum do not match!");

    for (size_t i = 0; i < currentGradient.size(); i++)
    {
        gradientSum.at(i) += currentGradient.at(i);
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

const std::vector<double> &Layer::backPropagate(const std::vector<double> &childGradient)
{
    if (currentGradient.size() != numParameters)
        Log::errAndQuit("Internal error: currentGradient array has incorrect size!");

    bprop(inputValues, childGradient, currentInputGradient, currentGradient);

    return currentInputGradient;
}

void Layer::computeDirect(const std::vector<double> &input, std::vector<double> &output) const
{
    output.resize(outputSize);

    op(input, output);
}

void Layer::setParameter(size_t id, double value)
{
    parameters.at(id) = value;
}

double Layer::getParameter(size_t id) const
{
    return parameters.at(id);
}


}
