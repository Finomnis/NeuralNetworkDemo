#include "InteractiveSubwindow.hxx"

#include "NeuralNetwork.hxx"

#include <iostream>
#include <ios>

namespace NeuralNetwork
{
InteractiveSubwindow::InteractiveSubwindow(std::shared_ptr<NeuralNetwork> nn)
    : neuralNetwork(nn)
{

}

InteractiveSubwindow::~InteractiveSubwindow()
{

}

void InteractiveSubwindow::render(SDL_Surface *surface)
{
    Uint32 col = SDL_MapRGB(surface->format, 0, 0, 0);
    SDL_FillRect(surface, &(surface->clip_rect), col);

    Uint32 red = SDL_MapRGB(surface->format, 255, 64, 64);
    Uint32 green = SDL_MapRGB(surface->format, 64, 255, 64);
    for (const auto &sample : samples)
    {
        float x = sample.inputs[0];
        float y = sample.inputs[1];
        float val = sample.outputs[0];
        int px = int((surface->clip_rect.w - 1) * x + 0.5f);
        int py = int((surface->clip_rect.h - 1) * y + 0.5f);
        SDL_Rect inner = {px - 3, py - 3, 7, 7};
        SDL_FillRect(surface, &inner, val > 0 ? red : green);
    }
}

void InteractiveSubwindow::onEvent(SDL_Event &event, SDL_Rect &subwindowSize)
{
    const Uint8 *keyboardState = SDL_GetKeyboardState(nullptr);

    bool continuous = keyboardState[SDL_SCANCODE_LSHIFT];
    bool remove = keyboardState[SDL_SCANCODE_LCTRL];

    SDL_Point mousePos;
    bool leftButton = false;
    bool rightButton = false;
    switch (event.type)
    {
        case SDL_MOUSEMOTION:
            if (!continuous)
                return;
            if (!isMouseInsideSubwindow(event.motion.x, event.motion.y, subwindowSize))
                return;
            mousePos = relativeMousePosition(event.motion.x, event.motion.y, subwindowSize);
            leftButton = ((event.motion.state & SDL_BUTTON_LMASK) != 0);
            rightButton = ((event.motion.state & SDL_BUTTON_RMASK) != 0);
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (!isMouseInsideSubwindow(event.button.x, event.button.y, subwindowSize))
                return;
            mousePos = relativeMousePosition(event.button.x, event.button.y, subwindowSize);
            leftButton = (event.button.button == SDL_BUTTON_LEFT);
            rightButton = (event.button.button == SDL_BUTTON_RIGHT);
            break;
        case SDL_MOUSEBUTTONUP:
            neuralNetwork->setTrainingData(samples);
            return;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_r)
            {
                samples.clear();
                neuralNetwork->setTrainingData(samples);
            }
            return;
        default:
            return;
    }

    if (remove)
    {
        if (leftButton || rightButton)
        {
            removeIntersectingSamples(subwindowSize, mousePos);
        }
    }
    else if (leftButton)
        addSample(subwindowSize, mousePos, 1.0f);
    else if (rightButton)
        addSample(subwindowSize, mousePos, -1.0f);

}

void
InteractiveSubwindow::removeIntersectingSamples(SDL_Rect &subwindowSize, SDL_Point mousePos)
{
    auto it = samples.begin();
    while (it != samples.end())
    {
        float x = it->inputs[0];
        float y = it->inputs[1];
        int px = int((subwindowSize.w - 1) * x + 0.5f);
        int py = int((subwindowSize.h - 1) * y + 0.5f);
        if (mousePos.x >= px - 3 && mousePos.x <= px + 3
                && mousePos.y >= py - 3 && mousePos.y <= py + 3)
            it = samples.erase(it);
        else
            it++;
    }
}

void
InteractiveSubwindow::addSample(SDL_Rect &subwindowSize, SDL_Point mousePos, float value)
{
    float x = mousePos.x / float(subwindowSize.w - 1);
    float y = mousePos.y / float(subwindowSize.h - 1);

    TrainingSample sample = {{x, y}, {value}};

    samples.emplace_back(std::move(sample));
}

}
