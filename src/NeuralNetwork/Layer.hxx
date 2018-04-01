#pragma once

#include <memory>
#include <vector>

namespace NeuralNetwork
{

class Layer
{
    public:
        Layer(size_t inputSize, size_t outputSize, size_t numParameters);
        ~Layer() = default;

        void setInputValues(const std::vector<float> &input);
        const std::vector<float> &getOutputValues() const;

        void setParameter(size_t id, float value);
        float getParameter(size_t id) const;

        void compute();
        void computeDirect(const std::vector<float> &input, std::vector<float> &output) const;
        const std::vector<float> &backPropagate(const std::vector<float> &childGradient);

        void finishCurrentTrainingSample();
        void walkGradient(float stepWidth);
        void clearTrainingData();

    protected:
        virtual void op(const std::vector<float> &input, std::vector<float> &output) const = 0;
        virtual void bprop(const std::vector<float> &input, const std::vector<float> &outputGradient, std::vector<float> &inputGradient, std::vector<float> &parameterGradient) const = 0;

    public:
        const size_t inputSize;
        const size_t outputSize;

    private:
        size_t numParameters;
        std::vector<float> inputValues;
        std::vector<float> outputValues;
        std::vector<float> parameters;
        std::vector<float> currentGradient;
        std::vector<float> currentInputGradient;
        std::vector<float> gradientSum;
        uint32_t numGradients;
};

}
