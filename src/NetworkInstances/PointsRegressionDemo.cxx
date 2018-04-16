#include "PointsRegressionDemo.hxx"

void PointsRegressionDemo::addTrainingSample(double x, double y)
{
    std::vector<double> in = {x};
    std::vector<double> out = {y};
    network->addTrainingSample(in, out);
}

void PointsRegressionDemo::createNetwork(std::vector<std::unique_ptr<NeuralNetwork::Layer>> &&layers,
        std::unique_ptr<NeuralNetwork::ErrorLayer> &&errorLayer)
{
    network = std::make_unique<NeuralNetwork::Network>(std::move(layers), std::move(errorLayer));
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
void convertPixelToAddress(SDL_Rect &rect, int px, int py, double &x, double &y)
{
    x = (double(px) / rect.w) * (MAX_X - MIN_X) + MIN_X;
    y = (double(py) / rect.h) * (MAX_Y - MIN_Y) + MIN_Y;
}

}

void PointsRegressionDemo::render(SDL_Renderer *renderer, SDL_Rect &rect)
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
        double preY = 0;
        for (int px = 0; px < rect.w; px++)
        {
            double x, dummy;
            convertPixelToAddress(rect, px, 0, x, dummy);

            double y = network->compute({x}).at(0);

            if (px != 0)
            {
                int px1, px2, py1, py2;
                convertAddressToPixel(rect, x - 1, preY, px1, py1);
                convertAddressToPixel(rect, x, y, px2, py2);
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_RenderDrawLine(renderer, px - 1, py1, px, py2);
            }

            preY = y;
        }
    }
}


void PointsRegressionDemo::onEvent(SDL_Event &event, SDL_Rect &rect)
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
            switch (event.key.keysym.sym)
            {
                case SDLK_ESCAPE:
                case SDLK_q:
                    SDL_Event sdlevent;
                    sdlevent.type = SDL_QUIT;
                    SDL_PushEvent(&sdlevent);
                    break;
                default:
                    break;
            }
            return;
        default:
            return;
    }
}
