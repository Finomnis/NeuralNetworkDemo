#include "MeanSquaredErrorLayer.hxx"

MeanSquaredErrorLayer::MeanSquaredErrorLayer(size_t inputSize):
    NeuralNetwork::ErrorLayer(inputSize)
{
}

void MeanSquaredErrorLayer::op(const std::vector<double> &input,
                               std::vector<double> &output) const
{
    const auto &expectedInput = getExpectedResult();

    double sum = 0.0;
    for (size_t i = 0; i < inputSize; i++)
    {
        double difference = input[i] - expectedInput[i];
        sum += difference * difference;
    }
    output[0] = sum;
}

void MeanSquaredErrorLayer::bprop(const std::vector<double> &input,
                                  const std::vector<double> &outputGradient,
                                  std::vector<double> &inputGradient,
                                  std::vector<double> &) const
{
    const auto &expectedInput = getExpectedResult();

    for (size_t i = 0; i < inputSize; i++)
    {
        inputGradient[i] = 2 * (input[i] - expectedInput[i]) * outputGradient[0];
    }
}















