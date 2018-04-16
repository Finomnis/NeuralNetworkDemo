#pragma once

#include "Layer.hxx"


namespace NeuralNetwork
{

class ErrorLayer : public Layer
{
    public:
        ErrorLayer(int inputSize);
        void setExpectedResult(const std::vector<double> &result);
    private:
        std::vector<double> expectedResult;
    protected:
        const std::vector<double> &getExpectedResult() const;
};

}
