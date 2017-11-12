#pragma once

#include "../SDL2Windows.hxx"

#include <memory>

namespace NeuralNetwork
{

class NeuralNetwork;

class InteractiveSubwindow : public SDL2Subwindow
{
    public:
        InteractiveSubwindow(std::shared_ptr<NeuralNetwork> nn);
        ~InteractiveSubwindow();

    private:
        std::shared_ptr<NeuralNetwork> neuralNetwork;
};

}
