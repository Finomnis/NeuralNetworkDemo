#pragma once

#include <memory>
#include <vector>

namespace NeuralNetwork
{

class Layer
{
    public:
        Layer(size_t inputSize, size_t outputSize, size_t numParameters);
        virtual ~Layer() = default;
        Layer(const Layer &) = delete;

        void setInputValues(const std::vector<double> &input);
        const std::vector<double> &getOutputValues() const;

        void setParameter(size_t id, double value);
        double getParameter(size_t id) const;

        void compute();
        void computeDirect(const std::vector<double> &input, std::vector<double> &output) const;
        const std::vector<double> &backPropagate(const std::vector<double> &childGradient);

        void finishCurrentTrainingSample();
        void walkGradient(double stepWidth);
        void clearTrainingData();

        double getRandom(double min, double max);

    protected:
        virtual void op(const std::vector<double> &input, std::vector<double> &output) const = 0;
        virtual void bprop(const std::vector<double> &input, const std::vector<double> &outputGradient, std::vector<double> &inputGradient, std::vector<double> &parameterGradient) const = 0;

    public:
        const size_t inputSize;
        const size_t outputSize;

    private:
        size_t numParameters;
        std::vector<double> inputValues;
        std::vector<double> outputValues;
        std::vector<double> parameters;
        std::vector<double> currentGradient;
        std::vector<double> currentInputGradient;
        std::vector<double> gradientSum;
        uint32_t numGradients;
};

}
