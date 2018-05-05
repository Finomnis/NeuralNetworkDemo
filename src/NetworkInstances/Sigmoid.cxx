#include "Sigmoid.hxx"

#include "../CommonLayers/SigmoidLayer.hxx"
#include "../CommonLayers/LeakyReluLayer.hxx"
#include "../CommonLayers/LinearLayer.hxx"
#include "../CommonLayers/BernoulliCrossEntropyErrorLayer.hxx"

#include <chrono>
#include <random>

Sigmoid::Sigmoid()
    : PointsRegressionDemo(0.05)
{
    std::vector<std::unique_ptr<NeuralNetwork::Layer>> layers;

    layers.emplace_back(std::make_unique<LinearLayer>(1, 5, 0.001));
    layers.emplace_back(std::make_unique<SigmoidLayer>(5));
    layers.emplace_back(std::make_unique<LinearLayer>(5, 5, 0.001));
    layers.emplace_back(std::make_unique<SigmoidLayer>(5));
    layers.emplace_back(std::make_unique<LinearLayer>(5, 1, 0.001));
    layers.emplace_back(std::make_unique<SigmoidLayer>(1));

    std::unique_ptr<NeuralNetwork::ErrorLayer> errorLayer = std::make_unique<BernoulliCrossEntropyErrorLayer>();

    createNetwork(std::move(layers), std::move(errorLayer));

    std::mt19937 rng(uint32_t(std::chrono::system_clock::now().time_since_epoch().count()));
    for (int i = 0; i < 500; i++)
    {
        std::uniform_real_distribution<double> uniform{0, 1};
        double x = uniform(rng);

        double val = uniform(rng);
        double y = (val > std::abs(2 * x - 1)) ? 0.0 : 1.0;

        addTrainingSample(x, y);
    }
}


