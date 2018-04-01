#include "NetworkConfigSubwindow.hxx"

#include "NeuralNetwork.hxx"

#include <iostream>
#include <ios>
#include <cmath>

namespace NeuralNetwork
{
NetworkConfigSubwindow::NetworkConfigSubwindow(std::shared_ptr<NeuralNetwork> nn)
    : neuralNetwork(nn)
{

}

NetworkConfigSubwindow::~NetworkConfigSubwindow()
{

}

namespace
{

void setValueColor(SDL_Renderer *renderer, float val)
{
    int col = std::min(int(std::fabs(val) * 20 + 0.5f), 255);

    if (val > 0)
        SDL_SetRenderDrawColor(renderer, 0, col, 0, 255);
    else
        SDL_SetRenderDrawColor(renderer, col, 0, 0, 255);
}

}

void NetworkConfigSubwindow::render(SDL_Renderer *renderer, SDL_Rect &rect)
{
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
    SDL_RenderFillRect(renderer, &rect);

    int leftBorder = 100;
    int topBorder = 100;

    int numLayers = neuralNetwork->getNumLayers();
    for (int i = 0; i < numLayers - 1; i++)
    {
        int neuron0Y = topBorder + rect.y + int((rect.h - 1 - topBorder) * (i + 1) / float(numLayers + 1) + 0.5f);
        int neuron1Y = topBorder + rect.y + int((rect.h - 1 - topBorder) * (i + 2) / float(numLayers + 1) + 0.5f);

        int layerSize0 = neuralNetwork->getLayerSize(i);
        int layerSize1 = neuralNetwork->getLayerSize(i + 1);

        for (int j = 0; j < layerSize0; j++)
        {
            int neuron0X = leftBorder + rect.x + int((rect.w - 1 - leftBorder) * (j + 1) / float(layerSize0 + 1) + 0.5f);
            for (int k = 0; k < layerSize1; k++)
            {
                int neuron1X = leftBorder + rect.x + int((rect.w - 1 - leftBorder) * (k + 1) / float(layerSize1 + 1) + 0.5f);
                float weight = neuralNetwork->getWeight(i + 1, j, k);

                setValueColor(renderer, weight);
                SDL_RenderDrawLine(renderer, neuron0X, neuron0Y, neuron1X, neuron1Y);
            }
        }
    }

    for (int i = 0; i < numLayers; i++)
    {
        int neuronY = topBorder + rect.y + int((rect.h - 1 - topBorder) * (i + 1) / float(numLayers + 1) + 0.5f);

        int layerSize = neuralNetwork->getLayerSize(i);
        for (int j = 0; j < layerSize; j++)
        {
            int neuronX = leftBorder + rect.x + int((rect.w - 1 - leftBorder) * (j + 1) / float(layerSize + 1) + 0.5f);

            SDL_Rect neuronRect = {neuronX - 3, neuronY - 3, 7, 7};
            SDL_Rect neuronOuterRect = {neuronX - 4, neuronY - 4, 9, 9};

            setValueColor(renderer, neuralNetwork->getBias(i, j) * 40);
            SDL_RenderFillRect(renderer, &neuronRect);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &neuronOuterRect);
        }
    }
}

void NetworkConfigSubwindow::onEvent(SDL_Event &event, SDL_Rect &subwindowSize)
{
    //SDL_Point mousePos;
    //bool leftButton = false;
    switch (event.type)
    {
        case SDL_MOUSEBUTTONDOWN:
            if (!isMouseInsideSubwindow(event.button.x, event.button.y, subwindowSize))
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


}
