#include "SmallestFullNetwork.hxx"

#include "../CommonLayers/LeakyReluLayer.hxx"
#include "../CommonLayers/LinearLayer.hxx"
#include "../CommonLayers/MeanSquaredErrorLayer.hxx"

#include <chrono>
#include <random>

/*
Pruned result:

Layer 0:
    -0.837064  2.469776
     0.941911 -0.068427
    -0.860254  1.660059
Layer 1: LeakyReluLayer
Layer 2:
     1.460613  0.942383  1.137542  -2.439625

Wolframalpha:
1.460613*max(0,-0.837064*x+2.469776) + 0.942383*max(0,0.941911*x-0.068427) + 1.137542*max(0,-0.860254*x+1.660059) - 2.439625 with 0<x<5
derive(1.460613*max(0,-0.837064*x+2.469776) + 0.942383*max(0,0.941911*x-0.068427) + 1.137542*max(0,-0.860254*x+1.660059) - 2.439625) by x

Derivatives (by Wolframalpha):
 -2.2012 | x<68427/941911
-1.31356 | 68427/941911<x<1660059/860254
-0.334986 | 1660059/860254<x<308722/104633
0.887641 | x>308722/104633

^=
-1.313560 | 0.07 < x < 1.93
-0.334986 | 1.93 < x < 2.95
 0.887641 | 2.95 < x

Input:
-1.3 | 0 < x < 2
-0.3 | 2 < x < 3
 0.9 | 3 < x
*/


SmallestFullNetwork::SmallestFullNetwork()
    : PointsRegressionDemo(0.005)
{
    std::vector<std::unique_ptr<NeuralNetwork::Layer>> layers;

    layers.emplace_back(std::make_unique<LinearLayer>(1, 15, 0.001, -1, 1, 0, 3));
    layers.emplace_back(std::make_unique<LeakyReluLayer>(15));
    layers.emplace_back(std::make_unique<LinearLayer>(15, 1, 0.001, -1, 1, 0, 3));

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


