#pragma once

#include "Layer.hxx"
#include "ErrorLayer.hxx"


namespace NeuralNetwork
{

class Network
{
    public:
        Network(std::vector<std::unique_ptr<Layer>> &&layers,
                std::unique_ptr<ErrorLayer> &&errorLayer);

        const std::vector<float> compute(const std::vector<float> &input);
        void addTrainingSample(const std::vector<float> &input, const std::vector<float> &output);
        float trainingStep(float stepWidth);
        float getParameter(size_t layerID, size_t parameterID) const;

    private:
        std::vector<std::unique_ptr<Layer>> layers;
        std::unique_ptr<ErrorLayer> errorLayer;

        std::vector<std::vector<float>> trainingInputs;
        std::vector<std::vector<float>> trainingOutputs;

};

}
