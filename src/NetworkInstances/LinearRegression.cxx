#include "LinearRegression.hxx"

#include "../CommonLayers/LinearLayer.hxx"
#include "../CommonLayers/MeanSquaredErrorLayer.hxx"

#include <chrono>
#include <random>

LinearRegression::LinearRegression()
    : PointsRegressionDemo(0.005)
{
    std::unique_ptr<NeuralNetwork::Layer> regressionLayer = std::make_unique<LinearLayer>(1, 1);
    std::unique_ptr<NeuralNetwork::ErrorLayer> errorLayer = std::make_unique<MeanSquaredErrorLayer>(1);

    std::vector<std::unique_ptr<NeuralNetwork::Layer>> layers;
    layers.push_back(std::move(regressionLayer));

    createNetwork(std::move(layers), std::move(errorLayer));

    std::mt19937 rng(uint32_t(std::chrono::system_clock::now().time_since_epoch().count()));
    for (int i = 0; i < 1000; i++)
    {
        std::normal_distribution<double> gauss{0, 0.2};
        std::uniform_real_distribution<double> uniform{0, 5};
        double x = uniform(rng);
        double y = (3 - 0.8 * x) + gauss(rng);
        //if (x > 2.5)
        //    y = (3 - 0.8 * (5 - x)) + gauss(rng);

        addTrainingSample(x, y);
    }
}


