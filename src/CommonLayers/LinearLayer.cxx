#include "LinearLayer.hxx"

LinearLayer::LinearLayer(size_t inputSize, size_t outputSize):
    NeuralNetwork::Layer(inputSize, outputSize, inputSize * outputSize + outputSize)
{
    const size_t numParametersPerOutput = inputSize + 1;
    for (size_t outId = 0; outId < outputSize; outId++)
    {
        for (size_t inId = 0; inId < inputSize; inId++)
        {
            setParameter(numParametersPerOutput * outId + inId, getRandom(-1, 1));
        }

        setParameter(numParametersPerOutput * outId + inputSize, getRandom(0, 3));
    }
}

void LinearLayer::op(const std::vector<double> &input,
                     std::vector<double> &output) const
{
    const size_t numParametersPerOutput = inputSize + 1;
    for (size_t outId = 0; outId < outputSize; outId++)
    {
        double sum = 0;

        for (size_t inId = 0; inId < inputSize; inId++)
        {
            double x = input.at(inId);
            double a = getParameter(numParametersPerOutput * outId + inId);
            sum += a * x;
        }

        double b = getParameter(numParametersPerOutput * outId + inputSize);
        sum += b;

        output.at(outId) = sum;
    }
}

void LinearLayer::bprop(const std::vector<double> &input,
                        const std::vector<double> &outputGradient,
                        std::vector<double> &inputGradient,
                        std::vector<double> &parameterGradient) const
{
    for (size_t inId = 0; inId < inputSize; inId++)
    {
        inputGradient.at(inId) = 0.0;
    }

    const size_t numParametersPerOutput = inputSize + 1;
    for (size_t outId = 0; outId < outputSize; outId++)
    {
        for (size_t inId = 0; inId < inputSize; inId++)
        {
            double x = input.at(inId);
            double a = getParameter(numParametersPerOutput * outId + inId);

            parameterGradient.at(numParametersPerOutput * outId + inId) = x * outputGradient.at(outId);
            inputGradient.at(inId) += a * outputGradient.at(outId);
        }

        //double b = getParameter(numParametersPerOutput * outId + inputSize);
        parameterGradient.at(numParametersPerOutput * outId + inputSize) = outputGradient.at(outId);
    }
}















