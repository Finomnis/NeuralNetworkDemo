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

        const std::vector<double> compute(const std::vector<double> &input);
        void addTrainingSample(const std::vector<double> &input, const std::vector<double> &output);
        double getNumTrainingSamples() const;
        size_t getNumLayers() const;
        size_t getNumParameters(size_t layerId) const;
        size_t getPreferredParameterGrouping(size_t layerId) const;
        const std::vector<double> &getTrainingInput(size_t id) const;
        const std::vector<double> &getTrainingOutput(size_t id) const;
        double trainingStep(double stepWidth);
        double getParameter(size_t layerID, size_t parameterID) const;

    private:
        std::vector<std::unique_ptr<Layer>> layers;
        std::unique_ptr<ErrorLayer> errorLayer;

        std::vector<std::vector<double>> trainingInputs;
        std::vector<std::vector<double>> trainingOutputs;

};

}
