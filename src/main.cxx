#include "Util/Log.hxx"
#include "NetworkInstances/Relu.hxx"
#include "Util/SDL2Windows.hxx"

#include <iostream>

#include <memory>
#include <vector>
#include <random>
#include <chrono>

int main(int argc, char *argv [])
{
    auto sdl2Initializer = std::make_shared<SDL2Initializer>();
    auto window = std::make_shared<SDL2Window>(sdl2Initializer, "NeuralNetworkDemo");

    std::vector<std::string> args;
    args.reserve(size_t(argc));
    for (int i = 0; i < argc; i++)
        args.emplace_back(argv[i]);

    Relu testNetwork;

    testNetwork.addToWindow(window, 0, 0, 1, 1);

    sdl2Initializer->runMainLoop();

    return 0;
}
