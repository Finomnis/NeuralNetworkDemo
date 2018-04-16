#include "ErrorLayer.hxx"

namespace NeuralNetwork
{

ErrorLayer::ErrorLayer(int inputSize)
    : Layer(inputSize, 1, 0),
      expectedResult(inputSize)
{

}
void ErrorLayer::setExpectedResult(const std::vector<double> &result)
{
    expectedResult = result;
}

const std::vector<double> &ErrorLayer::getExpectedResult() const
{
    return expectedResult;
}

}
