#include "LinearRegression.hxx"

#include <thread>
#include <chrono>
#include <iostream>

namespace
{
class LinearRegressionLayer : public NeuralNetwork::Layer
{
    public:
        LinearRegressionLayer(): NeuralNetwork::Layer(1, 1, 2)
        {
            setParameter(0, 1);
            setParameter(1, 0);
        }
    private:
        void op(const std::vector<float> &input,
                std::vector<float> &output) const override
        {
            float a = getParameter(0);
            float b = getParameter(1);
            float x = input[0];

            output[0] = a * x + b;
        }
        void bprop(const std::vector<float> &input,
                   const std::vector<float> &outputGradient,
                   std::vector<float> &inputGradient,
                   std::vector<float> &parameterGradient) const override
        {
            float a = getParameter(0);
            //float b = getParameter(1);
            float x = input[0];

            // d(out)/d(param)
            parameterGradient[0] = x * outputGradient[0];
            parameterGradient[1] = outputGradient[0];

            // d(out)/d(in)
            inputGradient[0] = a * outputGradient[0];
        }
};

class MeanSquaredErrorLayer : public NeuralNetwork::ErrorLayer
{
    public:
        MeanSquaredErrorLayer()
            : NeuralNetwork::ErrorLayer(1)
        {
        }
    private:
        void op(const std::vector<float> &input, std::vector<float> &output) const override
        {
            const auto &expectedInput = getExpectedResult();

            float sum = 0.0f;
            for (size_t i = 0; i < input.size(); i++)
            {
                float difference = input[i] - expectedInput[i];
                sum += difference * difference;
            }
            output[0] = sum;
        }
        void bprop(const std::vector<float> &input, const std::vector<float> &outputGradient, std::vector<float> &inputGradient, std::vector<float> &) const override
        {
            const auto &expectedInput = getExpectedResult();

            for (size_t i = 0; i < input.size(); i++)
            {
                inputGradient[i] = 2 * (input[i] - expectedInput[i]) * outputGradient[0];
            }
        }
};
}


LinearRegression::LinearRegression()
{
    std::unique_ptr<NeuralNetwork::Layer> regressionLayer = std::make_unique<LinearRegressionLayer>();
    std::unique_ptr<NeuralNetwork::ErrorLayer> errorLayer = std::make_unique<MeanSquaredErrorLayer>();

    std::vector<std::unique_ptr<NeuralNetwork::Layer>> layers;
    layers.push_back(std::move(regressionLayer));

    network = std::make_unique<NeuralNetwork::Network>(std::move(layers), std::move(errorLayer));
}



void LinearRegression::addTrainingSample(float x, float y)
{
    std::vector<float> in = {x};
    std::vector<float> out = {y};
    network->addTrainingSample(in, out);
}

void LinearRegression::run()
{
    float error = 1000;
    while (true)
    {
        //std::cout << "Current value at 1: " << network->compute(std::vector<float>({1})).at(0) << std::endl;
        //std::cout << "Current value at 3: " << network->compute(std::vector<float>({3})).at(0) << std::endl;
        //std::cout << "Current value at 4: " << network->compute(std::vector<float>({4})).at(0) << std::endl;
        float previousError = network->trainingStep(0.03);
        if (std::abs(previousError - error) < 0.00000001)
            break;
        error = previousError;
        std::cout << "Error before trainingStep: " << previousError << std::endl;
        //std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    float a = network->getParameter(0, 0);
    float b = network->getParameter(0, 1);
    std::cout << "Result: " << a << " * x " << (b < 0 ? "- " : "+ ") << std::abs(b) << std::endl;
}
