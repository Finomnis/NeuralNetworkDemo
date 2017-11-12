#include "SDL2Windows.hxx"
#include "NeuralNetwork/NeuralNetwork.hxx"
#include "NeuralNetwork/InteractiveSubwindow.hxx"

#include "Log.hxx"

#include <memory>

int main(int argc, char *argv[])
{
    auto sdl2Initializer = std::make_shared<SDL2Initializer>();

    auto window = std::make_shared<SDL2Window>(sdl2Initializer, "NeuralNetworkDemo");

    std::shared_ptr<NeuralNetwork::NeuralNetwork> neuralNetwork = std::make_shared<NeuralNetwork::NeuralNetwork>();

    NeuralNetwork::InteractiveSubwindow interactiveSubwindow(neuralNetwork);
    interactiveSubwindow.addToWindow(window, 0.3f, 0.4f, 0.6f, 0.7f);
    //window.addSubwindow(0.5f, 0, 1, 0.5f, NeuralNetwork::VisualizationSubwindow(neuralNetwork));
    //window.addSubwindow(0.5f, 0.5f, 1, 1, neuralNetwork::neuralNetworkSettingsSubwindow(neuralNetwork));

    Log::msg("Starting main loop ...");
    sdl2Initializer->runMainLoop();
    Log::msg("Main loop exited.");

    return 0;
}
