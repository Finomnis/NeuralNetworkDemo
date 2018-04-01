#include "Log.hxx"
#include "NetworkInstances/LinearRegression.hxx"

#include <iostream>

#include <memory>
#include <vector>

int main(int argc, char *argv [])
{
    std::vector<std::string> args;
    args.reserve(argc);
    for (int i = 0; i < argc; i++)
        args.emplace_back(argv[i]);

    LinearRegression testNetwork;
    testNetwork.addTrainingSample(1, -1);
    testNetwork.addTrainingSample(4, 1);
    testNetwork.addTrainingSample(3, 1);
    testNetwork.run();

    return 0;
}
