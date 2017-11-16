#include "InteractiveSubwindow.hxx"

#include "NeuralNetwork.hxx"

#include <iostream>
#include <ios>

#include <cmath>

namespace
{

Uint32 getValueColor(SDL_PixelFormat *format, float val)
{
    int col = std::min(int(std::fabs(val) * 128 + 0.5f), 128);

    if (val > 0)
        return SDL_MapRGB(format, 0, col, 0);
    else
        return SDL_MapRGB(format, col, 0, 0);
}

}

namespace NeuralNetwork
{
InteractiveSubwindow::InteractiveSubwindow(std::shared_ptr<NeuralNetwork> nn)
    : neuralNetwork(nn)
{
    SDL_Rect surfaceRect = {0, 0, 100, 100};
    networkOutputSurface = SDL_CreateRGBSurface(0, surfaceRect.w, surfaceRect.h, 32, 0x0000ff, 0x00ff00, 0xff0000, 0xff000000);
    if (networkOutputSurface == nullptr)
        Log::errAndQuit(SDL_GetError());
    Uint32 col = SDL_MapRGB(networkOutputSurface->format, 0, 0, 0);
    SDL_FillRect(networkOutputSurface, &surfaceRect, col);
}

InteractiveSubwindow::~InteractiveSubwindow()
{
    SDL_FreeSurface(networkOutputSurface);
}

void InteractiveSubwindow::renderNetworkTexture()
{
    for (int py = 0; py < networkOutputSurface->h; py++)
    {
        for (int px = 0; px < networkOutputSurface->w; px++)
        {
            float x = (px + 0.5f) / float(networkOutputSurface->w);
            float y = (py + 0.5f) / float(networkOutputSurface->h);
            float value = neuralNetwork->run({x, y})[0];

            Uint32 col = getValueColor(networkOutputSurface->format, value);

            Uint32 *pixel = (Uint32 *)((Uint8 *)(networkOutputSurface->pixels) + py * networkOutputSurface->pitch + px * networkOutputSurface->format->BytesPerPixel);
            *pixel = col;
        }
    }
}

void InteractiveSubwindow::render(SDL_Renderer *renderer, SDL_Rect &rect)
{
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
    SDL_RenderFillRect(renderer, &rect);

    renderNetworkTexture();

    {
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
        SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, networkOutputSurface);
        if (tex == nullptr)
            Log::errAndQuit(SDL_GetError());
        SDL_RenderCopy(renderer, tex, nullptr, &rect);
        SDL_DestroyTexture(tex);
    }

    for (const auto &sample : samples)
    {
        float x = sample.inputs[0];
        float y = sample.inputs[1];
        float val = sample.outputs[0];
        int px = rect.x + int((rect.w - 1) * x + 0.5f);
        int py = rect.y + int((rect.h - 1) * y + 0.5f);
        SDL_Rect inner = {px - 3, py - 3, 7, 7};
        if (val > 0)
            SDL_SetRenderDrawColor(renderer, 255, 64, 64, 255);
        else
            SDL_SetRenderDrawColor(renderer, 64, 255, 64, 255);
        SDL_RenderFillRect(renderer, &inner);
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
            if (trainingDataChanged)
            {
                neuralNetwork->setTrainingData(samples.begin(), samples.end());
                trainingDataChanged = false;
            }
            return;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_r)
            {
                samples.clear();
                neuralNetwork->setTrainingData(samples.begin(), samples.end());
                trainingDataChanged = false;
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
        {
            it = samples.erase(it);
            trainingDataChanged = true;
        }
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
    trainingDataChanged = true;
}

}
