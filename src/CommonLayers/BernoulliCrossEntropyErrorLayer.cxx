#include "BernoulliCrossEntropyErrorLayer.hxx"

#include <cmath>

BernoulliCrossEntropyErrorLayer::BernoulliCrossEntropyErrorLayer():
    NeuralNetwork::ErrorLayer(1)
{
}

void BernoulliCrossEntropyErrorLayer::op(const std::vector<double> &input,
        std::vector<double> &output) const
{
    const auto &expectedInput = getExpectedResult();

    double p = expectedInput.front();
    double q = input.front();

    output.front() = - p * std::log(q) - (1 - p) * std::log(1 - q);
}

void BernoulliCrossEntropyErrorLayer::bprop(const std::vector<double> &input,
        const std::vector<double> &outputGradient,
        std::vector<double> &inputGradient,
        std::vector<double> &) const
{
    const auto &expectedInput = getExpectedResult();

    double p = expectedInput.front();
    double q = input.front();

    inputGradient.front() = (p - q) / (q * q - q) * outputGradient[0];
}















