#include "SmallestFullNetwork.hxx"

#include "../CommonLayers/LeakyReluLayer.hxx"
#include "../CommonLayers/LinearLayer.hxx"
#include "../CommonLayers/MeanSquaredErrorLayer.hxx"

#include <chrono>
#include <random>

SmallestFullNetwork::SmallestFullNetwork()
    : PointsRegressionDemo(0.005)
{
    std::vector<std::unique_ptr<NeuralNetwork::Layer>> layers;

    layers.emplace_back(std::make_unique<LinearLayer>(1, 15, 0.001));
    layers.emplace_back(std::make_unique<LeakyReluLayer>(15));
    layers.emplace_back(std::make_unique<LinearLayer>(15, 1, 0.001));

    std::unique_ptr<NeuralNetwork::ErrorLayer> errorLayer = std::make_unique<MeanSquaredErrorLayer>(1);

    createNetwork(std::move(layers), std::move(errorLayer));

    std::mt19937 rng(uint32_t(std::chrono::system_clock::now().time_since_epoch().count()));
    for (int i = 0; i < 1000; i++)
    {
        std::normal_distribution<double> gauss{0, 0.2};
        std::uniform_real_distribution<double> uniform{0, 5};
        double x = uniform(rng);
        double y = 3.0 - 1.3 * x;
        if (x > 2)
            y = -0.3 * x + 1;
        if (x > 3)
            y = 0.9 * x - 2.6;

        y += gauss(rng);

        addTrainingSample(x, y);
    }
}


