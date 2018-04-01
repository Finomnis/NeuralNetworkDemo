#include "LinearRegression.hxx"

#include <thread>
#include <chrono>
#include <random>
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

    std::mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count());
    for (int i = 0; i < 1000; i++)
    {
        std::normal_distribution<float> gauss{0, 0.2};
        std::uniform_real_distribution<float> uniform{0, 5};
        float x = uniform(rng);
        float y = (3 - 0.8 * x) + gauss(rng);
        if (x > 2.5)
            y = (3 - 0.8 * (5 - x)) + gauss(rng);

        addTrainingSample(x, y);
    }
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


namespace
{

const float MIN_X = -1.0f;
const float MAX_X = 6.0f;
const float MIN_Y = -2.0f;
const float MAX_Y = 5.0f;
void convertAddressToPixel(SDL_Rect &rect, float x, float y, int &px, int &py)
{

    px = int((x - MIN_X) / (MAX_X - MIN_X) * rect.w + 0.5f);
    py = int((y - MIN_Y) / (MAX_Y - MIN_Y) * rect.h + 0.5f);
}

}

void LinearRegression::render(SDL_Renderer *renderer, SDL_Rect &rect)
{
    network->trainingStep(0.03);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);


    for (size_t i = 0; i < network->getNumTrainingSamples(); i++)
    {
        int px, py;

        float pointX = network->getTrainingInput(i)[0];
        float pointY = network->getTrainingOutput(i)[0];

        convertAddressToPixel(rect, pointX, pointY, px, py);

        SDL_Rect pointRect = {px - 1, py - 1, 3, 3};
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(renderer, &pointRect);
    }

    {
        float a = network->getParameter(0, 0);
        float b = network->getParameter(0, 1);
        int x0, y0, x1, y1;
        convertAddressToPixel(rect, MIN_X, a * MIN_X + b, x0, y0);
        convertAddressToPixel(rect, MAX_X, a * MAX_X + b, x1, y1);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawLine(renderer, x0, y0, x1, y1);
    }

}


void LinearRegression::onEvent(SDL_Event &event, SDL_Rect &rect)
{
    //SDL_Point mousePos;
    //bool leftButton = false;
    switch (event.type)
    {
        case SDL_MOUSEBUTTONDOWN:
            if (!isMouseInsideSubwindow(event.button.x, event.button.y, rect))
                return;
            //mousePos = relativeMousePosition(event.button.x, event.button.y, subwindowSize);
            //leftButton = (event.button.button == SDL_BUTTON_LEFT);
            break;
        case SDL_MOUSEBUTTONUP:
            return;
        case SDL_KEYDOWN:
            return;
        default:
            return;
    }
}
