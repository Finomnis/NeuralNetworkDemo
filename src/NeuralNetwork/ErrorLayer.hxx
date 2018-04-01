#pragma once

#include "Layer.hxx"


namespace NeuralNetwork
{

class ErrorLayer : public Layer
{
    public:
        ErrorLayer(int inputSize);
        void setExpectedResult(const std::vector<float> &result);
    private:
        std::vector<float> expectedResult;
    protected:
        const std::vector<float> &getExpectedResult() const;
};

}
