#include "SDL2Windows.hxx"
#include "NeuralNetwork/NeuralNetwork.hxx"
#include "NeuralNetwork/InteractiveSubwindow.hxx"
#include "NeuralNetwork/NetworkConfigSubwindow.hxx"

#include "Log.hxx"

#include <iostream>

#include <memory>

int main(int, char *[])
{
    auto sdl2Initializer = std::make_shared<SDL2Initializer>();

    auto window = std::make_shared<SDL2Window>(sdl2Initializer, "NeuralNetworkDemo");

    std::shared_ptr<NeuralNetwork::NeuralNetwork> neuralNetwork = std::make_shared<NeuralNetwork::NeuralNetwork>(2, 1);

    std::cout << "Neural network output: " << std::endl;
    for (const auto &val : neuralNetwork->run({0.0f, 0.0f}))
    {
        std::cout << val << std::endl;
    }


    NeuralNetwork::InteractiveSubwindow interactiveSubwindow(neuralNetwork);
    interactiveSubwindow.addToWindow(window, 0.0f, 0.0f, 0.6f, 1.0f);
    NeuralNetwork::NetworkConfigSubwindow networkConfigSubwindow(neuralNetwork);
    networkConfigSubwindow.addToWindow(window, 0.6f, 0.0f, 1.0f, 1.0f);

    Log::msg("Starting main loop ...");
    sdl2Initializer->runMainLoop();
    Log::msg("Main loop exited.");

    return 0;
}
