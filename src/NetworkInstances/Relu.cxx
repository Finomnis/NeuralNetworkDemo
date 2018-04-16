#include "Relu.hxx"

#include "../CommonLayers/ReluLayer.hxx"
#include "../CommonLayers/LinearLayer.hxx"
#include "../CommonLayers/MeanSquaredErrorLayer.hxx"

#include <chrono>
#include <random>

Relu::Relu()
{
    std::unique_ptr<NeuralNetwork::Layer> regressionLayer = std::make_unique<LinearLayer>(1, 1);
    std::unique_ptr<NeuralNetwork::Layer> reluLayer = std::make_unique<ReluLayer>(1);
    std::unique_ptr<NeuralNetwork::ErrorLayer> errorLayer = std::make_unique<MeanSquaredErrorLayer>(1);

    std::vector<std::unique_ptr<NeuralNetwork::Layer>> layers;
    layers.push_back(std::move(regressionLayer));
    layers.push_back(std::move(reluLayer));

    createNetwork(std::move(layers), std::move(errorLayer));

    std::mt19937 rng(uint32_t(std::chrono::system_clock::now().time_since_epoch().count()));
    for (int i = 0; i < 1000; i++)
    {
        std::normal_distribution<double> gauss{0, 0.2};
        std::uniform_real_distribution<double> uniform{0, 5};
        double x = uniform(rng);
        double y = 3.0 - 1.3 * x;
        if (y < 0)
            y = 0;

        y += gauss(rng);

        addTrainingSample(x, y);
    }
}


