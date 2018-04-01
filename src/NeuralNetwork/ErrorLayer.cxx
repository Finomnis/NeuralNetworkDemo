#include "ErrorLayer.hxx"

namespace NeuralNetwork
{

ErrorLayer::ErrorLayer(int inputSize)
    : Layer(inputSize, 1, 0),
      expectedResult(inputSize)
{

}
void ErrorLayer::setExpectedResult(const std::vector<float> &result)
{
    expectedResult = result;
}

const std::vector<float> &ErrorLayer::getExpectedResult() const
{
    return expectedResult;
}

}
