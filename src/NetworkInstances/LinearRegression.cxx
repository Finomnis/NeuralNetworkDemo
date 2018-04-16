#include "LinearRegression.hxx"

#include "../CommonLayers/LinearLayer.hxx"
#include "../CommonLayers/MeanSquaredErrorLayer.hxx"

#include <thread>
#include <chrono>
#include <random>
#include <iostream>



LinearRegression::LinearRegression()
{
    std::unique_ptr<NeuralNetwork::Layer> regressionLayer = std::make_unique<LinearLayer>(1, 1);
    std::unique_ptr<NeuralNetwork::ErrorLayer> errorLayer = std::make_unique<MeanSquaredErrorLayer>(1);

    std::vector<std::unique_ptr<NeuralNetwork::Layer>> layers;
    layers.push_back(std::move(regressionLayer));

    network = std::make_unique<NeuralNetwork::Network>(std::move(layers), std::move(errorLayer));

    std::mt19937 rng(uint32_t(std::chrono::system_clock::now().time_since_epoch().count()));
    for (int i = 0; i < 1000; i++)
    {
        std::normal_distribution<double> gauss{0, 0.2};
        std::uniform_real_distribution<double> uniform{0, 5};
        double x = uniform(rng);
        double y = (3 - 0.8 * x) + gauss(rng);
        //if (x > 2.5)
        //    y = (3 - 0.8 * (5 - x)) + gauss(rng);

        addTrainingSample(x, y);
    }
}



void LinearRegression::addTrainingSample(double x, double y)
{
    std::vector<double> in = {x};
    std::vector<double> out = {y};
    network->addTrainingSample(in, out);
}

void LinearRegression::run()
{
    double error = 1000;
    while (true)
    {
        //std::cout << "Current value at 1: " << network->compute(std::vector<double>({1})).at(0) << std::endl;
        //std::cout << "Current value at 3: " << network->compute(std::vector<double>({3})).at(0) << std::endl;
        //std::cout << "Current value at 4: " << network->compute(std::vector<double>({4})).at(0) << std::endl;
        double previousError = network->trainingStep(0.03);
        if (std::abs(previousError - error) < 0.00000001)
            break;
        error = previousError;
        std::cout << "Error before trainingStep: " << previousError << std::endl;
        //std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    double a = network->getParameter(0, 0);
    double b = network->getParameter(0, 1);
    std::cout << "Result: " << a << " * x " << (b < 0 ? "- " : "+ ") << std::abs(b) << std::endl;
}


namespace
{

const double MIN_X = -1.0;
const double MAX_X = 6.0;
const double MIN_Y = -2.0;
const double MAX_Y = 5.0;
void convertAddressToPixel(SDL_Rect &rect, double x, double y, int &px, int &py)
{

    px = int((x - MIN_X) / (MAX_X - MIN_X) * rect.w + 0.5);
    py = int((y - MIN_Y) / (MAX_Y - MIN_Y) * rect.h + 0.5);
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

        double pointX = network->getTrainingInput(i)[0];
        double pointY = network->getTrainingOutput(i)[0];

        convertAddressToPixel(rect, pointX, pointY, px, py);

        SDL_Rect pointRect = {px - 1, py - 1, 3, 3};
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(renderer, &pointRect);
    }

    {
        double a = network->getParameter(0, 0);
        double b = network->getParameter(0, 1);
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
